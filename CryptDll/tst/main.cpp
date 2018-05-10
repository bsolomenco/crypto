#include "CryptHelper.hpp"
#include "File.hpp"
#include <filesystem>
#include <functional>
#include <stdio.h>
#include <string.h>
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

    static CryptHelper ch(argv[1], buf, N);

    auto fnEncode = [](const char*fn){return ch.encodeStr(fn);};
    auto fnDecode = [](const char*fn){return ch.decodeStr(fn);};
    //auto& fnFunc = (restoreFlag ? fnDecode : fnEncode);
    const char* (*fnFunc)(const char*) = fnEncode;
    if(restoreFlag){
        fnFunc = fnDecode;
    }

    fs::path srcPath(argv[2]);
    if(fs::is_regular_file(srcPath)){
        const char* dstFilename = fnFunc(srcPath.filename().string().data());
        fs::path dstFilePath(dstDirPath / dstFilename);
        O("    \"%s\" -> \"%s\"\n", srcPath.string().data(), dstFilePath.string().data());
        File srcFile(srcPath.string().data(), "rb");
        File dstFile(dstFilePath.string().data(), "wb");
        ch(srcFile, dstFile);
        return 0;
    }

    //source is a directory
    int depth = 0;
    fs::path relPath;
    const char* dstFilename = nullptr;
    for(fs::recursive_directory_iterator i(srcPath), end; i!=end; ++i){
        dstFilename = fnFunc(i->path().filename().string().data());
        O("    depth=%d\n", i.depth());
        for(; depth>i.depth(); --depth){//back from a dir(s)
            relPath = relPath.parent_path();
        }
        depth = i.depth();
        if(fs::is_directory(i->path())){
            O("    DIR \"%s\" -> \"%s\"\n", i->path().string().data(), (dstDirPath/relPath/dstFilename).string().data());
            relPath /= dstFilename;
            fs::create_directories(dstDirPath / relPath);
        }else{
            O("    FILE \"%s\" -> \"%s\"\n", i->path().string().data(), (dstDirPath/relPath/dstFilename).string().data());
            File srcFile( i->path().string().data(), "rb");
            File dstFile((dstDirPath/relPath/dstFilename).string().data(), "wb");
            ch(srcFile, dstFile);
        }
    }

    return 0;
}
