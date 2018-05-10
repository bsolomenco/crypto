#pragma once

typedef void (* Init)(
    const unsigned char*        key,                    //key to encryrpt/decrypt (16 or 32 bytes long)
    size_t                      keyLen,                 //key's length (must be 16 or 32)
    const unsigned char*        iv,                     //initial vector(8, 12 or 20 bytes long)
    size_t                      ivLen                   //iv's length (must be 8,12 or 20)
);

typedef void (* Crypt)(void* buf, size_t len);

typedef size_t (* B64enc)(void* dst, size_t dim, const void* src, size_t len);
typedef size_t (* B64dec)(void* dst, size_t dim, const void* src, size_t len);
