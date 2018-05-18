#pragma once
#include <windows.h>
class MainWnd;

namespace Common{
    extern void*    mainWndId;
    extern void*    mainThrId;
    extern MainWnd* mainWnd;

    enum Msg{
        _0 = WM_USER    ,
        THR_STARTED     , 
        THR_STOPPED     , 
        THR_READY       ,
        THR_WORK        ,
        THR_PING        ,
        THR_PROGRESS    ,
    };
}