#pragma once
#include "Cipher.hpp"
#include <QString>
#include <filesystem>
namespace fs = std::experimental::filesystem;

const char* encodeStr(const char* str, Cipher& cipher, char* buf, size_t dim);
const char* decodeStr(const char* str, Cipher& cipher, char* buf, size_t dim);


//================================================================================
struct WorkInf{
    size_t  tix = 0;    //thread index
    size_t  tid = 0;    //thread ID

    size_t  fileSize = 0;   //file size
    size_t  len = 0;    //processed length
    double  percent = 0.0;

    fs::path srcPath;
    fs::path dstPath;

    Cipher  cipher;
    char    buf[1024*64];

    void set(fs::path src, fs::path dst, size_t size){
        srcPath = src;
        dstPath = dst;
        fileSize = size;
        len = 0;
        percent = 0;
    }
};

//================================================================================
class ThreadManager{
private:
    WorkInf arr[10] = {0};
    size_t  len=0;
public:
    ThreadManager();
    void start(unsigned threads, int op);
    int  stop(unsigned milliseconds);

    //int postMsg(unsigned msg, unsigned p0=0, unsigned p1=0);

    void ping(size_t tix);
    void pingAll();
    void encrypt(size_t tix);
    void quit(size_t tix);//post a WM_QUIT message to a thread


    static unsigned long workFunc(void* arg);
};