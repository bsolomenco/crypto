#include "CryptHelper.hpp"
#include "File.hpp"
#include <filesystem>
namespace fs = std::experimental::filesystem;
#define O(...) fprintf(stderr, __VA_ARGS__)
#define DBG(fmt, ...) O("DBG " fmt "[%s()]\n", __VA_ARGS__, __FUNCTION__)
#define ERR(fmt, ...) O("ERR " fmt "[%s()]\n", __VA_ARGS__, __FUNCTION__)
#define TRC(fmt, ...) O("TRC " fmt "[%s()]\n", __VA_ARGS__, __FUNCTION__)

namespace{
    //static const unsigned char key[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
    static const unsigned char key[32] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F};

    //static const unsigned char iv[]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    //static const unsigned char iv[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };

    //static const unsigned char iv[]  = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B};
    static const unsigned char iv[12] = {0};

    //static const unsigned char iv[]  = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13};//not working
}

//--------------------------------------------------------------------------------
const char* CryptHelper::encodeStr(const char* str){
	size_t len = strlen(str)*sizeof(str[0]);
	memcpy(_buf+1024, str, len+1);
    _init(key, sizeof(key)/sizeof(key[0]), iv, sizeof(iv)/sizeof(iv[0]));//set key & initial vector
    _crypt(_buf+1024, len);//encrypt filename
    len = _b64enc(_buf, _dim, _buf+1024, len);
    _buf[len] = 0;
    return _buf;
}

//--------------------------------------------------------------------------------
const char* CryptHelper::decodeStr(const char* str){
    size_t len = _b64dec(_buf, _dim, str, strlen(str));
    _init(key, sizeof(key)/sizeof(key[0]), iv, sizeof(iv)/sizeof(iv[0]));//set key & initial vector
    _crypt(_buf, len);//decrypt filename
    _buf[len] = 0;
    return _buf;
}

//--------------------------------------------------------------------------------
CryptHelper::CryptHelper(const char* dllFileName, void*buf, size_t len)
    : _dynLib(dllFileName)
    , _init(_dynLib("init"))
    , _crypt(_dynLib("crypt"))
    , _b64enc(_dynLib("b64enc"))
    , _b64dec(_dynLib("b64dec"))
    , _buf((char*)buf)
    , _dim(len)
{
}

//--------------------------------------------------------------------------------
int CryptHelper::operator()(FILE*srcFile, FILE*dstFile){
    _init(key, sizeof(key)/sizeof(key[0]), iv, sizeof(iv)/sizeof(iv[0]));//set key & initial vector
    for(size_t len=0; (len=fread(_buf, 1, _dim, srcFile))>0; ){//ToDo: _fread_nolock() ???
#if 0
        if(_progressCallback){
            if(_progressCallback(len))
                break;
        }
#endif
        _crypt(_buf, len);
        if(!fwrite(_buf, 1, len, dstFile)){//ToDo: _fwrite_nolock() ???
            break;
        }
    }
    return 0;
}

//--------------------------------------------------------------------------------
int CryptHelper::operator()(const char*src, const char*dst, Op op){
    const char* (CryptHelper::*fnFunc)(const char*) = (op==ENCRYPT ? &CryptHelper::encodeStr : &CryptHelper::decodeStr);
    fs::path dstDirPath(dst);
    if(!fs::is_directory(dstDirPath)){
        ERR("destination is not a directory: \"%s\"", dst);
        return -1;
    }

    fs::path srcPath(src);
    if(fs::is_regular_file(srcPath)){
        const char* dstFilename = (this->*fnFunc)(srcPath.filename().string().data());
        fs::path dstFilePath(dstDirPath / dstFilename);
        O("    \"%s\" -> \"%s\"\n", srcPath.string().data(), dstFilePath.string().data());
        File srcFile(srcPath.string().data(), "rb");
        File dstFile(dstFilePath.string().data(), "wb");
        operator()(srcFile, dstFile);
        return 0;
    }

    if(fs::is_directory(srcPath)){
        int depth = 0;
        fs::path relPath;
        const char* dstFilename = nullptr;
        for(fs::recursive_directory_iterator i(srcPath), end; i!=end; ++i){
            dstFilename = (this->*fnFunc)(i->path().filename().string().data());
            for(; depth>i.depth(); --depth){//back from a dir(s)
                relPath = relPath.parent_path();
            }
            depth = i.depth();
            if(fs::is_directory(i->path())){
                O("    DIR  \"%s\" -> \"%s\"\n", i->path().string().data(), (dstDirPath/relPath/dstFilename).string().data());
                relPath /= dstFilename;
                fs::create_directories(dstDirPath / relPath);
            }else{
                O("    FILE \"%s\" -> \"%s\"\n", i->path().string().data(), (dstDirPath/relPath/dstFilename).string().data());
                File srcFile( i->path().string().data(), "rb");
                File dstFile((dstDirPath/relPath/dstFilename).string().data(), "wb");
                operator()(srcFile, dstFile);
            }
        }
    }

    return 0;
}
