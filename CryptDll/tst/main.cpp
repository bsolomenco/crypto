#include "CryptHelper.hpp"
#include "File.hpp"
#include <filesystem>
#include <functional>
#include <stdio.h>
#include <string.h>
#include <time.h>
#define O(...) fprintf(stderr, __VA_ARGS__)
#define DBG(fmt, ...) O("DBG " fmt "[%s()]\n", __VA_ARGS__, __FUNCTION__)
#define ERR(fmt, ...) O("ERR " fmt "[%s()]\n", __VA_ARGS__, __FUNCTION__)
#define TRC(fmt, ...) O("TRC " fmt "[%s()]\n", __VA_ARGS__, __FUNCTION__)

namespace fs = std::experimental::filesystem;

//--------------------------------------------------------------------------------
int main(int argc, char** argv){
    if(argc < 4){
        O("SYNTAX: %s <library(.so/.dll)> <source file or dir> <destinationDir> -b|-r\n", strrchr(argv[0], '\\')+1);
        return -1;
    }
    bool restoreFlag = false;
    for(int i=0; i<argc; ++i){//display command line arguments
        O("    argv[%d] %s\n", i, argv[i]);
        if(strcmp(argv[i], "-b") == 0){
            restoreFlag = false;
        }else if(strcmp(argv[i], "-r") == 0){
            restoreFlag = true;
        }
    }
    fs::path dstDirPath(argv[3]);
    if(!fs::is_directory(dstDirPath)){
        ERR("destination is not a directory: \"%s\"", argv[2]);
        return -1;
    }

    constexpr size_t N = 1024 * 8;
    char buf[N] = {0};

    CryptHelper cryptHelper(argv[1], buf, N);
    time_t t0 = time(0);
    cryptHelper(argv[2], argv[3], restoreFlag ? CryptHelper::DECRYPT : CryptHelper::ENCRYPT);
    time_t t1 = time(0);
    fprintf(stderr, "TIM %lld sec\n", t1-t0);
    return 0;
}
