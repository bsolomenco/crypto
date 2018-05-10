#pragma once

namespace Base64{

    size_t encLen(size_t inputLen){
        size_t n = inputLen;
        if(n % 3){
            n += 3-(n%3);
        }
        return (n/3)*4;
        //return (n/3)<<2;
    }
    size_t decLen(size_t inputLen){
        size_t n = inputLen;
        if(n % 4){
            n += 4-(n%4);
        }
        return (n*3)/4;
    }

    size_t encode(char*dst, uint8_t*src, size_t len, bool finalInput);
    size_t decode(unsigned char*dst, char*src, size_t len, bool finalInput);
}
