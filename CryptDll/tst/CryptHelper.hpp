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
    size_t      _dim;
public:
    enum Op{ENCRYPT, DECRYPT};

    CryptHelper(const char*dllFileName, void*buf, size_t len);

    const char* encodeStr(const char*str);  //encrypt and base64 encode string; can be used for filenames
    const char* decodeStr(const char*str);  //encrypt and base64 decode string; can be used for filenames

    int operator()(FILE*srcFile, FILE*dstFile);//encrypt|decrypt source file into destination file
    int operator()(const char*src, const char*dst, Op op);//encrypt|decrypt source (file|dir) into destination directory
};

#if 0
Encrypt:
- dstFilename = b64enc(crypt(srcFilename.filename()));
- crypt(srcFilename, dstFilename)

Decrypt:
- dstFilename = b64dec(crypt(srcFilename.filename()));
- crypt(srcFilename, dstFilename)
#endif
