#if 0
Botan files generated using (in a VS2017 x64 command prompt):
    configure --amalgamation --single-amalgamation-file --minimized-build --enable-modules=chacha,chacha_sse2,base64,stream
#endif

#include "Cipher.hpp"
#include <stdio.h>

namespace X{
    const unsigned char*    key;    //key to encryrpt/decrypt (16 or 32 bytes long)
    size_t                  keyLen; //key's length (must be 16 or 32)
    const unsigned char*    iv;     //initial vector(8, 12 or 20 bytes long)
    size_t                  ivLen;  //iv's length (must be 8,12 or 20)

    Botan::ChaCha           cipher(20);
}

//--------------------------------------------------------------------------------
void Cipher::setKey(//sets encryption key & initial vector
    const unsigned char*        key,                    //key to encryrpt/decrypt (16 or 32 bytes long)
    size_t                      keyLen,                 //key's length (must be 16 or 32)
    const unsigned char*        iv,                     //initial vector(8, 12 or 20 bytes long)
    size_t                      ivLen                   //iv's length (must be 8,12 or 20)
){
	//TRC("");

	//checks
	//...

	X::key      = key;
	X::keyLen   = keyLen;
	X::iv       = iv;
	X::ivLen    = ivLen;
}

//--------------------------------------------------------------------------------
void Cipher::init(){//reinitialize
    //TRC("");
	_cipher.clear();
	_cipher.set_key(X::key, X::keyLen);
	_cipher.set_iv(X::iv, X::ivLen);
}

//--------------------------------------------------------------------------------
void Cipher::encrypt(void* buf, size_t len){//encrypt|decrypt buffer
    //TRC("");
    _cipher.cipher((unsigned char*)buf, (unsigned char*)buf, len);
}