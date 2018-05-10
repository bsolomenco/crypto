#include "CryptHelper.hpp"
#include "File.hpp"
#include <filesystem>
namespace fs = std::experimental::filesystem;

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
CryptHelper::CryptHelper(const char* dllFileName, void*buf, size_t len)
    : _dynLib(dllFileName)
    , _init(_dynLib("init"))
    , _crypt(_dynLib("crypt"))
    , _b64enc(_dynLib("b64enc"))
    , _b64dec(_dynLib("b64dec"))
    , _buf((char*)buf)
    , _len(len)
{
}

//--------------------------------------------------------------------------------
int CryptHelper::operator()(FILE* srcFile, FILE* dstFile){
    if(!srcFile){
        //ERR("cannot open source file: \"%s\"", dstFilePath.string().c_str());
        return -1;
    }
    if(!dstFile){
        //ERR("cannot open destination file: \"%s\"", dstFilePath.string().c_str());
        return -2;
    }

    //encrypt/decrypt file content
    constexpr size_t N = 1024*8;
    char buf[N] = {0};
    _init(key, sizeof(key)/sizeof(key[0]), iv, sizeof(iv)/sizeof(iv[0]));//(re)initialize cipher
    for(size_t len=0; (len=fread(buf, 1, N, srcFile))>0; ){
#if 0
        if(_progressCallback){
            if(_progressCallback(len))
                break;
        }
#endif
        _crypt(buf, len);
        if(!fwrite(buf, 1, len, dstFile)){
            break;
        }
    }

    return 0;
}

//--------------------------------------------------------------------------------
const char* CryptHelper::encodeStr(const char* str){
	size_t len = strlen(str)*sizeof(str[0]);
	memcpy(_buf+1024, str, len+1);
    _init(key, sizeof(key)/sizeof(key[0]), iv, sizeof(iv)/sizeof(iv[0]));
    _crypt(_buf+1024, len);//encrypt filename
    len = _b64enc(_buf, _len, _buf+1024, len);
    _buf[len] = 0;
    return _buf;
}

//--------------------------------------------------------------------------------
const char* CryptHelper::decodeStr(const char* str){
    size_t len = _b64dec(_buf, _len, str, strlen(str));
    _init(key, sizeof(key)/sizeof(key[0]), iv, sizeof(iv)/sizeof(iv[0]));
    _crypt(_buf, len);//decrypt filename
    _buf[len] = 0;
    return _buf;
}
