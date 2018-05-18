#include "ThreadManager.hpp"
#include "Base64.hpp"
#include "Common.hpp"
#include "File.hpp"
#include "MainWnd.hpp"
#include "Win_Thr.h"
#include <windows.h>

namespace{
    //static const unsigned char key[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
    static const unsigned char key[32] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F};

    //static const unsigned char iv[]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    //static const unsigned char iv[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };

    //static const unsigned char iv[]  = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B};
    static const unsigned char iv[12] = {0};

    //static const unsigned char iv[]  = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13};//not working
}


//--------------------------------------------------------------------------------
const char* encodeStr(const char* str, Cipher& cipher, char* buf, size_t dim){
	size_t len = strlen(str)*sizeof(str[0]);
	memcpy(buf+1024, str, len+1);
    cipher.init();
    cipher.encrypt(buf+1024, len);//encrypt filename
    len = Base64::encode(buf, dim, buf+1024, len);
    buf[len] = 0;
    return buf;
}

//--------------------------------------------------------------------------------
const char* decodeStr(const char* str, Cipher& cipher, char* buf, size_t dim){
    size_t len = Base64::decode(buf, dim, str, strlen(str));
    cipher.init();
    cipher.encrypt(buf, len);//decrypt filename
    buf[len] = 0;
    return buf;
}




//--------------------------------------------------------------------------------
ThreadManager::ThreadManager()
{
    Cipher::setKey(key, sizeof(key)/sizeof(key[0]), iv, sizeof(iv)/sizeof(iv[0]));
}

//--------------------------------------------------------------------------------
void ThreadManager::start(unsigned threads, int op){
    //qDebug("DBG PostThreadMessage(%08X, %d, %d)", _tid, p0, p1);
    len = threads;
    for(size_t i=sizeof(arr)/sizeof(arr[0]); i-->0; ){
        arr[i].tix = 0;
        arr[i].tid = 0;
        arr[i].srcPath = "";
        arr[i].dstPath = "";

        arr[i].fileSize = 0;
        arr[i].len      = 0;
        arr[i].percent  = 0.0;
    }
    for(size_t i=len; i-->0; ){
        arr[i].tix = i;
        arr[i].tid = Thr_run(workFunc, arr+i, 0, 0);
    }
}

//--------------------------------------------------------------------------------
int ThreadManager::stop(unsigned milliseconds){
    //qDebug("DBG PostThreadMessage(%08X, %d, %d)", _tid, p0, p1);
    for(size_t i=len; i-->0; ){
        if(arr[i].tid){
            PostThreadMessage((DWORD)arr[i].tid, WM_QUIT, 0, 0);
            Thr_wait((unsigned long)arr[i].tid, milliseconds);
        }
    }
    return 0;
}

//--------------------------------------------------------------------------------
void ThreadManager::ping(size_t tix){
    PostThreadMessage((DWORD)arr[tix].tid, Common::THR_PING, 0, 0);
}

//--------------------------------------------------------------------------------
void ThreadManager::pingAll(){
    for(size_t i=len; i-->0; ){
        PostThreadMessage((DWORD)arr[i].tid, Common::THR_PING, 0, 0);
    }
}

//--------------------------------------------------------------------------------
void ThreadManager::encrypt(size_t tix){
    PostThreadMessage((DWORD)arr[tix].tid, Common::THR_WORK, 0, 0);
}

//--------------------------------------------------------------------------------
void ThreadManager::quit(size_t tix){
    PostThreadMessage((DWORD)arr[tix].tid, WM_QUIT, 0, 0);
}

//================================================================================
unsigned long ThreadManager::workFunc(void* arg){
    size_t _tid = (size_t)QThread::currentThreadId();
    WorkInf& inf = *(WorkInf*)arg;

    qDebug("TRC threadId=%08X [%s()]", _tid, __FUNCTION__);

    MSG msg = {0};
    {//force message queue creation
        int err = PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
        qDebug("DBG %08X PeekMessage()=%d", _tid, err);
    }
    {//signal ready state
        //qDebug("%08X PostMessage(%p)", _tid, Common::mainWndId);
        int ok = PostMessage((HWND)Common::mainWndId, Common::THR_STARTED, (WPARAM)arg, 0);
        if(!ok){
            qDebug("ERR %08X PostMessage()", _tid);
        }
    }

    qDebug("DBG %08X waiting for messages...", _tid);
    while(GetMessage(&msg, HWND(0), 0, 0)){
        switch(msg.message){
            case Common::THR_PING:
                //qDebug("%08X(%d) THR_PING dst=\"%s\"", inf.tid, inf.tix, inf.dstPath.string().data());
                PostMessage((HWND)Common::mainWndId, Common::THR_READY, (WPARAM)arg, 0);
                break;
            case Common::THR_WORK:{
                //qDebug("%08X(%d) THR_WORK src=\"%s\" dst=\"%s\"", inf.tid, inf.tix, inf.srcPath.string().data(), inf.dstPath.string().data());

#if 1
                File srcFile(inf.srcPath.string().data(), "rb");
                File dstFile(inf.dstPath.string().data(), "wb");
                inf.cipher.init();
                for(size_t len=0; (len=fread(inf.buf, 1, sizeof(inf.buf), srcFile))>0; ){//ToDo: _fread_nolock() ???
#if 1
                    inf.len += len;
                    inf.percent = 100.0*inf.len/inf.fileSize;
                    PostMessage((HWND)Common::mainWndId, Common::THR_PROGRESS, (WPARAM)arg, len);
#endif
                    inf.cipher.encrypt(inf.buf, len);
                    if(!fwrite(inf.buf, 1, len, dstFile)){//ToDo: _fwrite_nolock() ???
                        break;
                    }
                }
#endif

                PostMessage((HWND)Common::mainWndId, Common::THR_READY, (WPARAM)arg, 0);
                break;
            }
            default:
                qDebug("msg=%u", msg.message);
        }
    }
    //WM_QUIT
    qDebug("%08X(%d) WM_QUIT", inf.tid, inf.tix);
    PostMessage((HWND)Common::mainWndId, Common::THR_STOPPED, (WPARAM)arg, 0);
    return 0;
}