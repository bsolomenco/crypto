#if 0 // Helper class to encrypt/decrypt
- based on: https://botan.randombit.net, https://github.com/randombit/botan
#endif
#pragma once
#include <stdio.h>
namespace Botan{class StreamCipher;}

class Cipher{
public:
    typedef int (ProgressCallback)(unsigned long long bytes);

    Cipher(
        const unsigned char*key,                    //key to encryrpt/decrypt (16 or 32 bytes long)
        size_t              keyLen,                 //key's length (must be 16 or 32)
        const unsigned char*iv,                     //initial vector(8, 12 or 20 bytes long)
        size_t              ivLen,                  //iv's length (must be 8,12 or 20)
        ProgressCallback*   callback=nullptr
    );
    ~Cipher();

    int  operator()(void* buf, size_t len);         //in-place encryption|decryption
    void operator()(FILE* srcFile, FILE* dstFile);  //use ferror(srcFile) & ferror(dstFile) to test for errors

protected:
    const unsigned char*    _key;
    size_t                  _keyLen;
    const unsigned char*    _iv;                //initial vector
    size_t                  _ivLen;

    Botan::StreamCipher*    _cipher;

    unsigned long long      _progressIncrement;
    ProgressCallback*       _progressCallback;
};
