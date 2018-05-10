#include "Crypto.hpp"

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

#define N 1024*1024*1024
    //--------------------------------------------------------------------------------
    int encrypt(const char* srcFilename, CryptoPP::Sink* sink, int (*cb)(unsigned long long bytes)=nullptr){//encrypt content of a file
        CryptoPP::AES::Encryption                       aesEnc(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
        CryptoPP::CBC_Mode_ExternalCipher::Encryption   cbcEnc(aesEnc, iv);
        if(cb){//advance manually with callback
            CryptoPP::FileSource fs(srcFilename, false, new CryptoPP::StreamTransformationFilter(cbcEnc, sink), true);
            while(!fs.SourceExhausted()){
                cb(fs.Pump(N));
            }
        }else{//no callback
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
    int decrypt(const char* srcFilename, CryptoPP::Sink* sink, int (*cb)(unsigned long long bytes)=nullptr){//decrypt content of a file
        CryptoPP::AES::Decryption                       aesDec(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
        CryptoPP::CBC_Mode_ExternalCipher::Decryption   cbcDec(aesDec, iv);
        if(cb){//advance manually with callback
            CryptoPP::FileSource fs(srcFilename, false, new CryptoPP::StreamTransformationFilter(cbcDec, sink), true);
            while(!fs.SourceExhausted()){
                cb(fs.Pump(N));
            }
        }else{//no callback
            CryptoPP::FileSource fs(srcFilename, true, new CryptoPP::StreamTransformationFilter(cbcDec, sink), true);
        }
        return 0;
    }

    //--------------------------------------------------------------------------------
    int decryptSalsa(const char* srcFilename, CryptoPP::Sink* sink){//decrypt content of a file
        CryptoPP::AES::Decryption       aesDec(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
        CryptoPP::Salsa20::Decryption   salsaDec(key, CryptoPP::AES::DEFAULT_KEYLENGTH, iv);
        CryptoPP::FileSource fs(srcFilename, true, new CryptoPP::StreamTransformationFilter(salsaDec, sink), true);
        return 0;
    }

    //--------------------------------------------------------------------------------
    int progressCallback(unsigned long long bytes){
        static unsigned long long progressCounter = 0;
        if(!bytes)
            progressCounter = 0;
        else
            progressCounter += bytes;
        printf("%llu\n", progressCounter);
        return 0;
    }
}

//================================================================================
int main(int argc, char* argv[]) {
    printf("TRC [%s(argc=%d)]\n", __FUNCTION__, argc);

    enum class Command{NONE, ENCRYPT, DECRYPT};
    Command command = Command::NONE;

    enum class Flags{NONE, PROGRESS=(1<<0)};
    unsigned flags = (unsigned)Flags::NONE;

    const char* srcFilename = nullptr;
    const char* dstFilename = nullptr;

    {//process command line
        int fileIndex[2] = {0};
        int fileCount = 0;
        for(int i=1; i<argc; ++i){
            unsigned arg = 0;
            for(const char*p=argv[i] ; *p; arg<<=8,arg|=*p++);
            switch(arg){
                case 'E':
                    if(command != Command::NONE){printf("ERR multiple commands\n"); return -1;}
                    command = Command::ENCRYPT;
                    break;
                case 'D':
                    if(command != Command::NONE){printf("ERR multiple commands\n"); return -1;}
                    command = Command::DECRYPT;
                    break;
                case '-p':
                    flags |= (unsigned)Flags::PROGRESS;
                    break;
                default://source & destination files
                    if(fileCount > 1){printf("ERR too many filenames\n"); return -1;}
                    fileIndex[fileCount++] = i;
            }
        }
        if(command == Command::NONE){
            printf("ERR missing command\n");
            return -1;
        }
        if(fileCount != 2){
            printf("ERR missing source or destination files\n");
            return -1;
        }
        srcFilename = argv[fileIndex[0]];
        dstFilename = argv[fileIndex[1]];
    }
    if(argc < 4){
        printf("SYNTAX: %s <command{E, D}> <options{-p}> <sourceFilename> <destinationFilename>\n", argv[0]);
        return 0;
    }
    printf("key[%d]: ", CryptoPP::AES::DEFAULT_KEYLENGTH); dumpHex(key, CryptoPP::AES::DEFAULT_KEYLENGTH); printf("\n");
    printf("iv [%d]: ", CryptoPP::AES::BLOCKSIZE); dumpHex(iv, CryptoPP::AES::BLOCKSIZE); printf("\n");

    Crypto crypto(1024*1024, flags&(unsigned)Flags::PROGRESS ? &progressCallback : nullptr);

    switch(command){
        case Command::ENCRYPT:{
            printf("encrypting \"%s\" --> \"%s\" ...\n", srcFilename, dstFilename);
            time_t t0 = time(0);
            crypto.encrypt(srcFilename, dstFilename);
            time_t t1 = time(0);
            printf("TIM %lld sec\n", t1-t0);
            break;
        }
        case Command::DECRYPT:{
            printf("decrypting \"%s\" -> \"%s\" ...\n", srcFilename, dstFilename);
            time_t t0 = time(0);
            crypto.decrypt(srcFilename, dstFilename);
            time_t t1 = time(0);
            printf("TIM %lld sec\n", t1-t0);
            break;
        }
        default:
            printf("ERR unknown command: %s\n", argv[1]);
    }
    printf("TRC .[%s()]\n", __FUNCTION__);
    return 0;
}
