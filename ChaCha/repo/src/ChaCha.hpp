#pragma once
#include <memory.h>
#include <stdint.h>

class ChaCha{
protected:
    
public:
    ChaCha();

    void setKey(const uint8_t*key, size_t len);
    void setIv(const uint8_t*iv, size_t len);

    void operator()(uint8_t*buf, size_t len);//encrypt|decrypt in place
};
