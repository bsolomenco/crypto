#include "FileSystemThr.hpp"
#include <windows.h>

//--------------------------------------------------------------------------------
FileSystemThr::FileSystemThr::FileSystemThr(QObject*parent)
    : QThread(parent)
{
}

//--------------------------------------------------------------------------------
int FileSystemThr::postMsg(unsigned msg, unsigned p0, unsigned p1){
    return PostThreadMessage((DWORD)_threadId, msg, p0, p1);
}

//--------------------------------------------------------------------------------
void FileSystemThr::run(){
    _threadId = (size_t)currentThreadId();
    qDebug("TRC threadId=%d [%s()]", _threadId, __FUNCTION__);
    MSG msg = {0};
    {//force message queue creation
        //PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE)
        int err = PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
        qDebug("DBG PeekMessage()=%d", err);
    }
    //while(GetMessage(&msg, HWND(0), WM_USER, WM_USER)){
    while(GetMessage(&msg, HWND(0), 0, 0)){
        switch(msg.message){
            case WM_DESTROY:
                 qDebug("WM_DESTROY");
                 PostQuitMessage(0);
                 break;
            case WM_QUIT:
                 qDebug("WM_QUIT");
                 goto E;
            case ENCODECODE:
                qDebug("ENCODECODE src=\"%d\" dst=\"%d\"", _src.toUtf8().constData(), _dst.toUtf8().constData());
                _dirIter = fs::recursive_directory_iterator(_src.toUtf8().data());
                break;
            default:
                qDebug("msg=%u", msg.message);
        }
    }
E:  //cleanup & exit
    qDebug("%s().", __FUNCTION__);
}

//--------------------------------------------------------------------------------
void FileSystemThr::post(QString src, QString dst, int op){
    _src = src;
    _dst = dst;
    postMsg(ENCODECODE, op);
}