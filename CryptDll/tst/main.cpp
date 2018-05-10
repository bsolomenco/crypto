#include "CryptHelper.hpp"
#include <stdio.h>
#include <string.h>
#include <filesystem>
#define O(...) fprintf(stderr, __VA_ARGS__)
#define DBG(fmt, ...) O("DBG " fmt "[%s()]\n", __VA_ARGS__, __FUNCTION__)
#define ERR(fmt, ...) O("ERR " fmt "[%s()]\n", __VA_ARGS__, __FUNCTION__)
#define TRC(fmt, ...) O("TRC " fmt "[%s()]\n", __VA_ARGS__, __FUNCTION__)

namespace fs = std::experimental::filesystem;

//--------------------------------------------------------------------------------
int main(int argc, char** argv){
    if(argc < 4){
        O("SYNTAX: %s <library(.so/.dll)> <destinationDir> <filename> [-d]\n", strrchr(argv[0], '\\')+1);
        return -1;
    }
    bool decodeFlag = false;
    for(int i=0; i<argc; ++i){//display command line arguments
        O("    argv[%d] %s\n", i, argv[i]);
        if(strcmp(argv[i], "-d") == 0){
            decodeFlag = true;
        }
    }
    fs::path dstDirPath(argv[2]);
    if(!fs::is_directory(dstDirPath)){
        ERR("destination is not a path: \"%s\"", argv[2]);
        return -1;
    }

    constexpr size_t N = 1024 * 8;
    char buf[N] = {0};

    for(int i=3; i<argc; ++i){
        static CryptHelper ch(argv[1], buf, N);

        fs::path srcFilePath(argv[i]);
        if(!fs::is_regular_file(srcFilePath)){
            ERR("not a regular file: \"%s\"\n", argv[i]);
            return -i;
        }

        const char* dstFilename = (decodeFlag ? ch.decodeStr(srcFilePath.filename().string().c_str()) : ch.encodeStr(srcFilePath.filename().string().c_str()));
        fs::path dstFilePath(dstDirPath / dstFilename);
        O("    \"%s\" -> \"%s\"\n", argv[i], dstFilePath.string().c_str());
        ch(srcFilePath.string().c_str(), dstFilePath.string().c_str());
    }
    return 0;
}
