#pragma once
#include "Crypt.hpp"
#include "DynLib.hpp"
#include <stdio.h>
#include <string>

class CryptHelper{
protected:
    DynLib      _dynLib;
    Init        _init;
    Crypt       _crypt;
    B64enc      _b64enc;
    B64enc      _b64dec;
    char*       _buf;
    size_t      _len;
public:
    CryptHelper(const char* dllFileName, void*buf, size_t len);

    int operator()(FILE* srcFile, FILE* dstFile);//encrypt|decrypt source file into destination file

    const char* encodeStr(const char*str);  //encrypt and base64 encode string; can be used for filenames
    const char* decodeStr(const char*str);  //encrypt and base64 decode string; can be used for filenames
};

#if 0
Encrypt:
- dstFilename = b64enc(crypt(srcFilename.filename()));
- crypt(srcFilename, dstFilename)

Decrypt:
- dstFilename = b64dec(crypt(srcFilename.filename()));
- crypt(srcFilename, dstFilename)
#endif
