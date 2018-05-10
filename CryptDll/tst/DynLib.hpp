#pragma once    //2011-05-01 � bSolo

class DynLib{//Dynamic Library loader|helper (for: .dll, .so)
protected:
    char    _file[1024];//dynamic library file name (as UTF8)
    void*   _hndl;      //library handle
    void*   _func;      //function pointer
public:
    DynLib(const char* const dlFileName=0);//ctor; doesn't load the DLL, only memorize the name of the dynamic library to be used when loading symbols
    ~DynLib();//dtor

    void setFileName(const char* const fileName);      //[unload and] only memorize the name of the dynamic library to be used when loading symbols
    DynLib& operator()(const char* const symbol);      //gets symbol (function or variable) address by name, stores it into _func member then it can be used via operator T()
    template<typename T> operator T(){return (T)_func;}//conversion operator to type T; works together with operator()(const char*) to cast automagically
};

#if 0 //USAGE --------------------------------------------------------------------------------
    //declare somewhere all DLLs needed (no loading at this time)
    DynLib oneDl("One.dll");
    DynLib twoDl("Two.dll");

    char libNameUTF8[1024] = {0};
    WideCharToMultiByte(CP_UTF8, 0, L"\u53F0\u5317", -1, libNameUTF8, sizeof(libNameUTF8)/sizeof(libNameUTF8[0]), 0, 0);//convert Unicode widechar to UTF8
    DynLib dynLib(libNameUTF8);//UTF8 encoded Unicode filename

    //load and call functions from DLL (1st fct load will load DLL if not already loaded)
    ReturnType1 (*oneFunc)(ParameterType1) = oneDl("oneFunc");
    if(!oneFunc){/*ERR*/}
    ParameterType1 par1;
    ReturnType1 ret1 = oneFunc(par1);//execute function "oneFunc" from "One.dll"

    ReturnType2 (*twoFunc)(ParameterType2) = twoDl("oneFunc");
    if(!twoFun){/*ERR*/}
    ParameterType2 par2;
    ReturnType2 ret2 = twoFunc(par);//execute function "oneFunc" from "Two.dll"
#endif