#include "Cipher.hpp"
#include "botan_all.h"

//--------------------------------------------------------------------------------
Cipher::Cipher(const unsigned char*key, size_t keyLen, const unsigned char*iv, size_t ivLen, ProgressCallback* callback)
    : _key(key)
    , _keyLen(keyLen)
    , _iv(iv)
    , _ivLen(ivLen)
  //, _cipher(Botan::StreamCipher::create("ChaCha(20)"))
  //, _cipher(Botan::StreamCipher::create("ChaCha20"))
    , _cipher(new Botan::ChaCha(20))
    , _progressCallback(callback)
{
   _cipher->set_key(_key, _keyLen);
   _cipher->set_iv(_iv,ivLen);
}

//--------------------------------------------------------------------------------
Cipher::~Cipher(){
    delete _cipher;
    _cipher = nullptr;
}

//--------------------------------------------------------------------------------
int Cipher::operator()(void* buf, size_t len){
    _cipher->cipher((unsigned char*)buf, (unsigned char*)buf, len);
    return 0;
}

//--------------------------------------------------------------------------------
void Cipher::operator()(FILE* srcFile, FILE* dstFile){
    constexpr size_t N = 1024*1024*8;
    static unsigned char buf[N] = {0};
    if(_progressCallback){
        _progressCallback(0);
    }
    for(size_t len=0; (len=fread(buf, 1, N, srcFile))>0; ){
        if(_progressCallback){
            if(_progressCallback(len))
                break;
        }
        _cipher->cipher(buf, buf, len);
        if(!fwrite(buf, 1, len, dstFile)){
            break;
        }
    }
}
