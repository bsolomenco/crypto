#if 0 // Helper class to encrypt/decrypt
- based on: https://www.cryptopp.com/ , https://github.com/weidai11/cryptopp
#endif
#pragma once
namespace CryptoPP{class Sink;};

class Crypto{
public:
    typedef int (ProgressCallback)(unsigned long long bytes);

    Crypto(unsigned long long progressIncrement=1024*1024, ProgressCallback* callback=nullptr);
    ~Crypto();

    int encrypt(const char* srcFilename, const char* dstFilename);
    int decrypt(const char* srcFilename, const char* dstFilename);

protected:
    //Key and IV setup
    //AES encryption uses a secret key of a variable length (128-bit, 196-bit or 256-bit). This key is secretly exchanged between two parties before communication begins. DEFAULT_KEYLENGTH=16 bytes
    unsigned char key[16] = {"symmetric key"};
    unsigned char iv[16]  = {"initial vector"};

    unsigned long long  _progressIncrement;
    ProgressCallback*   _progressCallback;

    int _encrypt(const char* filename, CryptoPP::Sink* sink);
    int _decrypt(const char* filename, CryptoPP::Sink* sink);
};
