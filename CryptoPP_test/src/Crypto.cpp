#include "Crypto.hpp"
#include "aes.h"
#include "modes.h"
#include "files.h"
#include "filters.h"

//--------------------------------------------------------------------------------
Crypto::Crypto(unsigned long long progressIncrement, ProgressCallback* callback)
    : _progressIncrement(progressIncrement)
    , _progressCallback(callback)
{
}

//--------------------------------------------------------------------------------
Crypto::~Crypto()
{}

//--------------------------------------------------------------------------------
int Crypto::encrypt(const char* srcFilename, const char* dstFilename){
    return _encrypt(srcFilename, new CryptoPP::FileSink(dstFilename));
}

//--------------------------------------------------------------------------------
int Crypto::decrypt(const char* srcFilename, const char* dstFilename){
    return _decrypt(srcFilename, new CryptoPP::FileSink(dstFilename));
}

//--------------------------------------------------------------------------------
int Crypto::_encrypt(const char* filename, CryptoPP::Sink* sink){
    CryptoPP::AES::Encryption                       aesEnc(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Encryption   cbcEnc(aesEnc, iv);
    if(_progressCallback){//advance manually with callback
        _progressCallback(0);
        CryptoPP::FileSource fs(filename, false, new CryptoPP::StreamTransformationFilter(cbcEnc, sink), true);
        while(!fs.SourceExhausted()){
            _progressCallback(fs.Pump(_progressIncrement));
        }
        _progressCallback(fs.Pump(_progressIncrement));
    }else{//no callback
        CryptoPP::FileSource fs(filename, true, new CryptoPP::StreamTransformationFilter(cbcEnc, sink), true);
    }
    return 0;
}

//--------------------------------------------------------------------------------
int Crypto::_decrypt(const char* filename, CryptoPP::Sink* sink){
    CryptoPP::AES::Decryption                       aesDec(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Decryption   cbcDec(aesDec, iv);
    if(_progressCallback){//advance manually with callback
        _progressCallback(0);
        CryptoPP::FileSource fs(filename, false, new CryptoPP::StreamTransformationFilter(cbcDec, sink), true);
        while(!fs.SourceExhausted()){
            _progressCallback(fs.Pump(_progressIncrement));
        }
    }else{//no callback
        CryptoPP::FileSource fs(filename, true, new CryptoPP::StreamTransformationFilter(cbcDec, sink), true);
    }
    return 0;
}
