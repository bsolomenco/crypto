#if 0
Botan files generated using (in a VS2017 x64 command prompt):
    configure --amalgamation --single-amalgamation-file --minimized-build --enable-modules=chacha,chacha_sse2,base64,stream
#endif

#include "botan_all.h"
#include "Crypt.hpp"
#include <stdio.h>

namespace X{
    const unsigned char*    key;    //key to encryrpt/decrypt (16 or 32 bytes long)
    size_t                  keyLen; //key's length (must be 16 or 32)
    const unsigned char*    iv;     //initial vector(8, 12 or 20 bytes long)
    size_t                  ivLen;  //iv's length (must be 8,12 or 20)

    Botan::ChaCha           cipher(20);
}

extern "C"{//export undecorated names

    //--------------------------------------------------------------------------------
    void __stdcall init(//sets encryption key & initial vector
        const unsigned char*        key,                    //key to encryrpt/decrypt (16 or 32 bytes long)
        size_t                      keyLen,                 //key's length (must be 16 or 32)
        const unsigned char*        iv,                     //initial vector(8, 12 or 20 bytes long)
        size_t                      ivLen                   //iv's length (must be 8,12 or 20)
    ){
		#pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__ )
		//TRC("");

		//checks
		//...

		X::key      = key;
		X::keyLen   = keyLen;
		X::iv       = iv;
		X::ivLen    = ivLen;

		X::cipher.clear();
		X::cipher.set_key(X::key, X::keyLen);
		X::cipher.set_iv(X::iv, X::ivLen);
    }

    //--------------------------------------------------------------------------------
    void __stdcall crypt(void* buf, size_t len){//encrypt|decrypt buffer
        #pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__ )
        //TRC("");
        X::cipher.cipher((unsigned char*)buf, (unsigned char*)buf, len);
    }

    //--------------------------------------------------------------------------------
    size_t __stdcall b64enc(char* dst, size_t dim, const void* src, size_t len){
        #pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__ )
        //TRC("");
        return Botan::base64_encode(dst, (const unsigned char*)src, len, dim, true);
    }

    //--------------------------------------------------------------------------------
    size_t __stdcall b64dec(void*dst, size_t dim, const char*src, size_t len){
        #pragma comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__ )
        //TRC("");
        return Botan::base64_decode((unsigned char*)dst, src, len, dim, true);
    }

}//extern "C"
