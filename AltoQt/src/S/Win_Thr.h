//THRead - compiles on C & C++, works also on WinCE
#pragma once

//================================================== C-std
#ifdef __cplusplus
extern "C"{
#endif
    typedef unsigned long (__stdcall *Thr_Fct)(void* par);//prototype of a function to be executed in a thread (=ThreadProc)

    unsigned long __stdcall Thr_run(//starts a function in a thread
        Thr_Fct        fct,     //function to run in a separate thread
        void*          param,   //parameters for fct
        unsigned long  stack,   //stack size (0=default)
        int            priority //thread priority (0=normal)
    );                          //return: tid>0, err=0 (use GetLastError())

    int __stdcall Thr_wait(//wait for a thread to end
        unsigned long   tid,    //threads ID (eg. returned by Thr_run())
        unsigned long   timeout //milliseconds to wait
	);                          //return err!=0, ok==0
    int __stdcall Thr_waitEx(//wait for a set of threads to end
        unsigned long*  tid,    //threads ID (eg. returned by Thr_run())
        size_t          len, 
        unsigned long   timeout //milliseconds to wait
	);                          //return err!=0, ok==0

#ifdef __cplusplus
}//extern "C"{
#endif


//================================================== C++
#ifdef __cplusplus
#include <functional>
namespace Win{
    namespace Thr{
        extern "C" unsigned long __stdcall run(void* ptr);//Windows thread procedure (ThreadProc)

        inline void runx(
            std::function<unsigned long()>& func,
            unsigned long                   stack = 0UL,    //stack size (0=default)
            int                             priority = 0    //thread priority (0=normal)
        ){
            Thr_run(&Win::Thr::run, &func, stack, priority);
        }
    }
}
#endif//#ifdef __cplusplus


//================================================== USAGE
#if 0//usage C-std
    unsigned long tid = Thr_run(...);
    if(!tid){}//ERR
    else Thr_wait(tid);
#endif
#if 0//usage C++
    Cls{
        unsigned long met(){...}
    } obj;
    unsigned long tid = Thr_run(obj, &Cls::met);
    if(!tid){ERR(...);}
    else{Thr_wait(tid, timeout);}//wait for the thread to finish
#endif