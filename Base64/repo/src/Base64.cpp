#include "Base64.hpp"
#include <stdint.h>
#include <string>
/**
* Round up
* @param n a non-negative integer
* @param align_to the alignment boundary
* @return n rounded up to a multiple of align_to
*/
inline size_t round_up(size_t n, size_t align_to)
   {
   //BOTAN_ASSERT(align_to != 0, "align_to must not be 0");

   if(n % align_to)
      n += align_to - (n % align_to);
   return n;
   }

namespace{
    static const uint8_t BIN_TO_BASE64[64] = {
       'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
       'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
       'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
       'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '(', ')',
    };

    void do_base64_encode(char out[4], const uint8_t in[3]){
       out[0] = BIN_TO_BASE64[(in[0] & 0xFC) >> 2];
       out[1] = BIN_TO_BASE64[((in[0] & 0x03) << 4) | (in[1] >> 4)];
       out[2] = BIN_TO_BASE64[((in[1] & 0x0F) << 2) | (in[2] >> 6)];
       out[3] = BIN_TO_BASE64[in[2] & 0x3F];
    }
}

//--------------------------------------------------------------------------------
size_t Base64::encode(char*dst, uint8_t*src, size_t len, bool finalInput){
    size_t input_consumed = 0;

    size_t input_remaining = len;
    size_t output_produced = 0;

    while(input_remaining >= 3){
        do_base64_encode(dst + output_produced, src + input_consumed);
        input_consumed += 3;
        output_produced += 4;
        input_remaining -= 3;
    }

    if(finalInput && input_remaining){
        uint8_t remainder[3] = { 0 };
        for(size_t i = 0; i != input_remaining; ++i){
            remainder[i] = src[input_consumed + i];
        }

        do_base64_encode(dst + output_produced, remainder);

        size_t empty_bits = 8 * (3 - input_remaining);
        size_t index = output_produced + 4 - 1;
        while(empty_bits >= 8){
            dst[index--] = '=';
            empty_bits -= 6;
        }

        input_consumed += input_remaining;
        output_produced += 4;
    }

    return output_produced;
}

//--------------------------------------------------------------------------------
size_t Base64::decode(unsigned char*dst, char*src, size_t len, bool finalInput){
    /*
     * Base64 Decoder Lookup Table
     * Warning: assumes ASCII encodings
    */
    static const uint8_t BASE64_TO_BIN[256] = {
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x80, 0xFF, 0xFF, 0x80, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0x80, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3E, 0xFF, 0xFF, 0xFF, 0x3F,
        0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0xFF, 0xFF, 0xFF, 0x81, 0xFF, 0xFF,
        0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
        0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
        0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    };

    uint8_t* out_ptr = dst;
    uint8_t decode_buf[4];
    size_t decode_buf_pos = 0;
    size_t final_truncate = 0;

    //clear_mem(dst, len * 3 / 4);
    memset(dst, 0, len*3/4);

    for(size_t i=0; i!=len; ++i){
        const uint8_t bin = BASE64_TO_BIN[static_cast<uint8_t>(src[i])];
        if(bin <= 0x3F){
            decode_buf[decode_buf_pos] = bin;
            decode_buf_pos += 1;
        }else if(!(bin == 0x81 || (bin == 0x80 && /*ignore_ws*/false))){
            std::string bad_char(1, src[i]);
            if(bad_char == "\t")
                bad_char = "\\t";
            else if(bad_char == "\n")
                bad_char = "\\n";
            else if(bad_char == "\r")
                bad_char = "\\r";
         //ERR
#if 0
         throw Invalid_Argument(
           std::string("base64_decode: invalid base64 character '") +
           bad_char + "'");
#endif
        }

        //If we're at the end of the input, pad with 0s and truncate
        if(finalInput && (i == len-1)){
            if(decode_buf_pos){
                for(size_t j = decode_buf_pos; j != 4; ++j){
                    decode_buf[j] = 0;
                }
                final_truncate = (4 - decode_buf_pos);
                decode_buf_pos = 4;
            }
        }

        if(decode_buf_pos == 4){
            out_ptr[0] = (decode_buf[0] << 2) | (decode_buf[1] >> 4);
            out_ptr[1] = (decode_buf[1] << 4) | (decode_buf[2] >> 2);
            out_ptr[2] = (decode_buf[2] << 6) | decode_buf[3];

            out_ptr += 3;
            decode_buf_pos = 0;
            input_consumed = i+1;
        }
    }

    while(input_consumed < len && BASE64_TO_BIN[static_cast<uint8_t>(src[input_consumed])] == 0x80){
        ++input_consumed;
     }

    size_t written = (out_ptr - dst) - final_truncate;

    return written;
}
