#include "aes.h"
#include "files.h"
#include "filters.h"
#include "modes.h"
#include "salsa.h"
#include "seckey.h"
#include <stdio.h>
#include <conio.h>
#include <time.h>

//================================================================================
namespace{
    //Key and IV setup
    //AES encryption uses a secret key of a variable length (128-bit, 196-bit or 256-   
    //bit). This key is secretly exchanged between two parties before communication   
    //begins. DEFAULT_KEYLENGTH= 16 bytes
    unsigned char key[CryptoPP::AES::DEFAULT_KEYLENGTH] = {"symmetric key"};
    unsigned char iv[CryptoPP::AES::BLOCKSIZE]          = {"initial vector"};

    //--------------------------------------------------------------------------------
    void dumpHex(const void* buf, size_t len){
        unsigned char* b = (unsigned char*)buf;
        for(size_t i=0; i<len; ++i){
            printf("%02X ", b[i]);
        }
    }

    //--------------------------------------------------------------------------------
    int encrypt(const void* src, size_t len, CryptoPP::Sink* sink){//encrypt content of a buffer
        // Create Cipher Text
        CryptoPP::AES::Encryption aesEncryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
        CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, iv);
        CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, sink);
        stfEncryptor.Put(reinterpret_cast<const unsigned char*>(src), len);
        stfEncryptor.MessageEnd();
        return 0;
    }

    //--------------------------------------------------------------------------------
    int encrypt(const char* srcFilename, CryptoPP::Sink* sink, int (*cb)(size_t bytes)=nullptr){//encrypt content of a file
        CryptoPP::AES::Encryption                       aesEnc(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
        CryptoPP::CBC_Mode_ExternalCipher::Encryption   cbcEnc(aesEnc, iv);
        if(cb){//advance manually with callback
            CryptoPP::FileSource fs(srcFilename, false, new CryptoPP::StreamTransformationFilter(cbcEnc, sink), true);
            while(!fs.SourceExhausted()){
                fs.Pump(1024*1024);
                cb(1024*1024);
            }
        }
        else{
            CryptoPP::FileSource fs(srcFilename, true, new CryptoPP::StreamTransformationFilter(cbcEnc, sink), true);
        }
        return 0;
    }

    //--------------------------------------------------------------------------------
    int encryptSalsa(const char* srcFilename, CryptoPP::Sink* sink){//encrypt content of a file
        CryptoPP::AES::Encryption       aesEnc(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
        CryptoPP::Salsa20::Encryption   salsaEnc(key, CryptoPP::AES::DEFAULT_KEYLENGTH, iv);
        CryptoPP::FileSource fs(srcFilename, true, new CryptoPP::StreamTransformationFilter(salsaEnc, sink), true);
        return 0;
    }

    //--------------------------------------------------------------------------------
    int decrypt(const void* src, size_t len, CryptoPP::Sink* sink){//decrypt content of a buffer
        CryptoPP::AES::Decryption                       aesDec(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
        CryptoPP::CBC_Mode_ExternalCipher::Decryption   cbcDec(aesDec, iv);
        CryptoPP::StreamTransformationFilter            stfDec(cbcDec, sink);
        stfDec.Put(reinterpret_cast<const unsigned char*>(src), len);
        stfDec.MessageEnd();
        return 0;
    }

    //--------------------------------------------------------------------------------
    int decrypt(const char* srcFilename, CryptoPP::Sink* sink){//decrypt content of a file
        CryptoPP::AES::Decryption                       aesDec(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
        CryptoPP::CBC_Mode_ExternalCipher::Decryption   cbcDec(aesDec, iv);
        CryptoPP::FileSource fs(srcFilename, true, new CryptoPP::StreamTransformationFilter(cbcDec, sink), true);
        return 0;
    }

    //--------------------------------------------------------------------------------
    int decryptSalsa(const char* srcFilename, CryptoPP::Sink* sink){//decrypt content of a file
        CryptoPP::AES::Decryption       aesDec(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
        CryptoPP::Salsa20::Decryption   salsaDec(key, CryptoPP::AES::DEFAULT_KEYLENGTH, iv);
        CryptoPP::FileSource fs(srcFilename, true, new CryptoPP::StreamTransformationFilter(salsaDec, sink), true);
        return 0;
    }
}

//================================================================================
int main(int argc, char* argv[]) {
    printf("TRC [%s(argc=%d)]\n", __FUNCTION__, argc);
    if(argc < 4){
        printf("SYNTAX: %s <command{e, d}> <sourceFilename> <destinationFilename>\n", argv[0]);
        return 0;
    }
    switch(argv[1][0]){
        case 'e':
        case 'E':{//encrypt
            printf("encrypting \"%s\" --> \"%s\" ...\n", argv[2], argv[3]);
            time_t t0 = time(0);
            encrypt(argv[2], new CryptoPP::FileSink(argv[3]));
            time_t t1 = time(0);
            printf("TIM %lld sec\n", t1-t0);
            break;
        }
        case 'd':
        case 'D':{//decrypt
            printf("decrypting \"%s\" -> \"%s\" ...\n", argv[2], argv[3]);
            time_t t0 = time(0);
            decrypt(argv[2], new CryptoPP::FileSink(argv[3]));
            time_t t1 = time(0);
            printf("TIM %lld sec\n", t1-t0);
            break;
        }
        default:
            printf("ERR unknown command: %s\n", argv[1]);
    }
    return 0;

    printf("key[%d]: ", CryptoPP::AES::DEFAULT_KEYLENGTH); dumpHex(key, CryptoPP::AES::DEFAULT_KEYLENGTH); printf("\n");
    printf("iv [%d]: ", CryptoPP::AES::BLOCKSIZE); dumpHex(iv, CryptoPP::AES::BLOCKSIZE); printf("\n");

    // String and Sink setup
    const char txt[] = {"Text to be encrypted..."};

    // Dump Plain Text
    printf("txt[%zd]: ", strlen(txt)); dumpHex(txt, strlen(txt)); printf("\n");

    std::string encryptedStr;
    {//encrypt
        //encrypt(txt, strlen(txt)+1, new CryptoPP::StringSink(encryptedStr));
        //printf("enc[%zd]: ", encryptedStr.size()); dumpHex(encryptedStr.c_str(), encryptedStr.size()); printf("\n");
        //encrypt(txt, strlen(txt)+1, new CryptoPP::FileSink("C:\\Prj\\cryptopp.enc"));

        printf("encrypting \"%s\" ...\n", argv[1]);
        time_t t0 = time(0);
        encrypt(argv[1], new CryptoPP::FileSink(argv[2]));
        time_t t1 = time(0);
        printf("TIM t1-t0=%lld\n", t1-t0);
    }
    {//decrypt
        std::string out;
        //decrypt(encryptedStr.c_str(), encryptedStr.size(), new CryptoPP::StringSink(out));//decrypt to a std::string
        //printf("dec[%zd]: ", out.size()); dumpHex(out.c_str(), out.size()); printf("\n");
        //printf("%s\n", out.c_str());
        //decrypt(encryptedStr.c_str(), encryptedStr.size(), new CryptoPP::FileSink("C:\\Prj\\cryptopp.dec"));//decrypt to a file

        printf("decrypting \"%s\" ...\n", argv[2]);
        time_t t0 = time(0);
        decrypt(argv[2], new CryptoPP::FileSink("C:\\Prj\\cryptopp.dec"));//decrypt to a file
        time_t t1 = time(0);
        printf("TIM t1-t0=%lld\n", t1-t0);
    }

    printf("TRC .[%s()]\n", __FUNCTION__);
    return 0;
}
