#pragma once
#include "botan_all.h"

class Cipher{
private:
    Botan::ChaCha   _cipher;
public:
    static void setKey(
        const unsigned char*    key,        //key to encryrpt/decrypt (16 or 32 bytes long)
        size_t                  keyLen,     //key's length (must be 16 or 32)
        const unsigned char*    iv,         //initial vector(8, 12 or 20 bytes long)
        size_t                  ivLen       //iv's length (must be 8,12 or 20)
    );

    Cipher(): _cipher(20){}
    void init();
    void Cipher::encrypt(void* buf, size_t len);
};