#pragma once

namespace Base64{
    size_t encode(char*dst, size_t dim, const void*src, size_t len);
    size_t decode(void*dst, size_t dim, const char*src, size_t len);
}