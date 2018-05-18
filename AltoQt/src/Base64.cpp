#include "Base64.hpp"
#include "botan_all.h"
#include <stdio.h>

//--------------------------------------------------------------------------------
size_t Base64::encode(char*dst, size_t dim, const void*src, size_t len){
    return Botan::base64_encode(dst, (const unsigned char*)src, len, dim, true);
}

//--------------------------------------------------------------------------------
size_t Base64::decode(void*dst, size_t dim, const char*src, size_t len){
        return Botan::base64_decode((unsigned char*)dst, src, len, dim, true);
}