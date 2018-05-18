//compiles also on C-std
#include "Win_Thr.h"
#ifdef O_
    #include "O.h"
#else
    #define O(...)
#endif
#include <Windows.h>
#pragma warning(push, 4)//set warning level

//================================================== C-std

//--------------------------------------------------
unsigned long __stdcall Thr_run(Thr_Fct fct, void* param, unsigned long stack, int priority){
    unsigned long tid = 0;
    void*         thr = 0;
    if(!fct){
        O("ERR (fct=0) invalid parameter");
        return 0;
    }

    thr = CreateThread(0, stack, fct, param, CREATE_SUSPENDED, (DWORD*)&tid);
    if(!thr){
        O("ERR CreateThread() GetLastError()=%u", GetLastError());
        return 0;
    }
    O("DBG thr=%p tid=%ul(%Xh)", thr, tid, tid);

    for(;;){
        if(priority){//set priority
            if(!SetThreadPriority(thr, priority)){
                O("ERR SetThreadPriority() GetLastError()=%u", GetLastError());
                break;
            }
        }
        //start thread
        if(ResumeThread(thr) == 0xFFFFFFFF){
            O("ERR ResumeThread(thr=%p) GetLastError()=%u", thr, GetLastError());
            break;
        }
        break;
    }//for(; ; ){

    if(thr){
        if(!CloseHandle(thr)){
            O("ERR CloseHandle(%p) GetLastError()=%u", thr, GetLastError());
        }
    }
    return tid;
}

//--------------------------------------------------
int __stdcall Thr_wait(unsigned long tid, unsigned long timeout){
#ifdef _WIN32_WCE
    HANDLE thr = (HANDLE)tid;
#else
    HANDLE thr = OpenThread(SYNCHRONIZE, 0, tid);
#endif
    int err = 0;
    if(!thr){
        O("WRN (tid=%p ms=%u) thr=0", tid, timeout);
        //Sleep(timeout);
        err = 1;
    }else{
        switch(err = WaitForSingleObject(thr, timeout)){
            case WAIT_ABANDONED:
                O("WRN (tid=%p ms=%d) Wait(thr=%p)=%u(WAIT_ABANDONED)", tid, timeout, thr, err);
                err = 0;
                break;
            case WAIT_OBJECT_0:
                O("DBG (tid=%p) Wait(thr=%p)=%u(WAIT_OBJECT_0)", tid, thr, err);
                break;
            case WAIT_FAILED:
                O("ERR (tid=%p ms=%d) Wait(thr=%p)=%u(WAIT_FAILED) GetLastError()=%u", tid, timeout, thr, err, GetLastError());
                err = -2;
                break;
            case WAIT_TIMEOUT:
                O("WRN (tid=%p ms=%d) Wait(thr=%p)=%u(TIMEOUT)", tid, timeout, thr, err);
                err = -3;
                break;
            default:
                O("WRN (tid=%p ms=%d) Wait(thr=%p)=%u(???)", tid, timeout, thr, err);
        }//switch
        CloseHandle(thr); thr=0;
    }
    return err;
}

//--------------------------------------------------
int __stdcall Thr_waitEx(unsigned long*tid, size_t len, unsigned long timeout){
    return 0;
}

//================================================== C++ only
#ifdef __cplusplus

//--------------------------------------------------
unsigned long __stdcall Win::Thr::run(void* ptr){//Windows thread procedure (ThreadProc)
    unsigned long err = 0xFFFFFFFF;
    if(!ptr){
        //O("ERR "__FUNCTION__"(ptr=0) invalid parameter");
    }else{
        //Functor<unsigned long()>* fct = reinterpret_cast<Functor<unsigned long()>*>(ptr);
        //err = (*fct)();
        //delete fct; fct=0;

        auto func = static_cast<std::function<unsigned long()>*>(ptr);
        (*func)();
    }
    return err;
}

#endif//#ifdef __cplusplus

#pragma warning(pop)//restore warning level