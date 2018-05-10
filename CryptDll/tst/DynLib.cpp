#include "DynLib.hpp"
#ifdef _WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif
#include <string.h>

#if 1 //debug
#include <stdio.h>
#define O(...) fprintf(stderr, __VA_ARGS__)
#else
#define O(...)
#endif
#define DBG(fmt, ...) O("DBG " fmt "[%s()]\n", __VA_ARGS__, __FUNCTION__)
#define ERR(fmt, ...) O("ERR " fmt "[%s()]\n", __VA_ARGS__, __FUNCTION__)
#define TRC(fmt, ...) O("TRC " fmt "[%s()]\n", __VA_ARGS__, __FUNCTION__)

//--------------------------------------------------------------------------------
DynLib::DynLib(const char* const dlFileName)//ctor
    : _file()
    , _hndl(0)
    , _func(0)
{
    TRC("%s(fileName=\"%s\")", __func__, dlFileName);
    if(dlFileName)
        setFileName(dlFileName);
}

//--------------------------------------------------------------------------------
DynLib::~DynLib(){//dtor
    if(_hndl){
        int err = 0;
#ifdef _WIN32
        if(!FreeLibrary(HMODULE(_hndl))){
            err = ::GetLastError();
#else //Linux
        if((err = ::dlclose(_hndl))){
#endif
            ERR("unload dynamic library: \"%s\" (err=%d)", _file, err);
        }
        else
            DBG("unload dynamic library: \"%s\"", _file);
        _hndl = 0;
    }
    _file[0] = 0;
    _func = 0;
}

//--------------------------------------------------------------------------------
void DynLib::setFileName(const char* const dlFileName){
    TRC("%s(fileName=\"%s\")", __func__, dlFileName);
    if(_hndl && strcmp(_file, dlFileName)){//something loaded already -> unload it
        this->~DynLib();
    }
    if(dlFileName)
        strncpy(_file, dlFileName, sizeof(_file)/sizeof(_file[0]));
}

//--------------------------------------------------------------------------------
DynLib& DynLib::operator()(const char* const functionName){
    if(!_hndl){
#ifdef _WIN32//Windows
        wchar_t fn[sizeof(_file)/sizeof(_file[0])] = {0};
        //mbstowcs(fn, _file, sizeof(_file)/sizeof(_file[0]));//doesn't work properly
        ::MultiByteToWideChar(CP_UTF8, 0, _file, -1, fn, sizeof(fn)/sizeof(fn[0]));
        _hndl = ::LoadLibraryW(fn);
#elif __linux__ //Linux
        _hndl = ::dlopen(_file, RTLD_LAZY);
#endif
        if(!_hndl){
            ERR("load dynamic library: \"%s\"", _file);
            return *this;
        }
        {//full path & filename
#ifdef _WIN32
            fn[0] = 0;
            DBG("load dynamic library: \"%S\"", (GetModuleFileNameW(HMODULE(_hndl), fn, sizeof(fn)/sizeof(fn[0])), fn));
#elif __linux__ //Linux
            DBG("load dynamic library: \"%s\"", _file);
#endif
        }
    }

#ifdef _WIN32
    _func = GetProcAddress((HMODULE)_hndl, functionName);
#elif __linux__ //Linux
    _func = ::dlsym(_hndl, functionName);
#endif
    if(_func)
        DBG("symb \"%s\" \"%s\"", _file, functionName);
    else
        ERR("symb \"%s\" \"%s\"", _file, functionName);

    return *this;
}
