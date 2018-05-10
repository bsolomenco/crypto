#include "Cipher.hpp"
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <time.h>

//--------------------------------------------------------------------------------
const char* hex(const void* inp, size_t len, char* out){
   static const char a[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
   for(size_t i=0; i<len; ++i){
      unsigned char x = ((unsigned char*)inp)[i];
      *out++ = a[(x>>4)&0x0F];
      *out++ = a[(x   )&0x0F];
   }
    return out;
}

//--------------------------------------------------------------------------------
int callback(size_t bytes){
    static size_t counter = 0;
    //counter += bytes;
    if(bytes == 0)
        counter = 0;
    else
        counter += bytes;
    fprintf(stderr, "%llu\n", counter);
    return 0;
}

//================================================================================
int main(int argc, char** argv){
    //printf("TRC [%s(argc=%d)]\n", __FUNCTION__, argc);

    enum class Flags{NONE, PROGRESS=(1<<0)};
    unsigned flags = (unsigned)Flags::NONE;

    {//process command line
        static const char syntax[] = {"command flags sourceFile destinationFile"};
        //command
        for(int i=1; i<argc; ++i){
            unsigned arg = 0;
            for(const char*p=argv[i]; *p; arg<<=8,arg|=*p++);//max 4 chars command to little endian unsigned
            switch(arg){
                case '-p': flags |= (unsigned)Flags::PROGRESS; break;
                case '-h': printf("%s [-p]\n", argv[0]); return 0;
                //default  : printf("ERR unknown flag|option: %s\n", argv[i]); return -2;
            }
        }
    }

    const unsigned char key16[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
    const unsigned char key32[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F};
    const unsigned char iv[]  = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
  //const unsigned char iv[]  = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B};
  //const unsigned char iv[]  = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13};//not working
#if 0
    //generate fresh nonce (IV)
    std::unique_ptr<Botan::RandomNumberGenerator> rng(new Botan::AutoSeeded_RNG);
    std::vector<uint8_t> iv(8);
    rng->randomize(iv.data(),iv.size());
#endif
    Cipher cipher(key16, sizeof(key16)/sizeof(key16[0]), iv, sizeof(iv)/sizeof(iv[0]), flags&(unsigned)Flags::PROGRESS ? &callback : nullptr); 

    //put stdin & stdout in binary mode
    int err = 0;
    err = _setmode(_fileno(stdin), _O_BINARY);
    if(err < 0){
        fprintf(stderr, "ERR _setmode(stdin)=%d\n", err);
    }
    err = _setmode(_fileno(stdout), _O_BINARY);
    if(err < 0){
        fprintf(stderr, "ERR _setmode(stdout)=%d\n", err);
    }

    time_t t0 = time(0);
    cipher(stdin, stdout);
    time_t t1 = time(0);
    fprintf(stderr, "TIM %lld sec\n", t1-t0);

    if(ferror(stdin)){
        fprintf(stderr, "ERR ferror(stdin)=%d\n", err);
    }
    if(ferror(stdout)){
        fprintf(stderr, "ERR ferror(stdout)=%d\n", err);
    }

    fprintf(stderr, "TRC .[%s()]\n", __FUNCTION__);
   return 0;
}
