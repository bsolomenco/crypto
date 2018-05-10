#include "ChaCha.hpp"

//--------------------------------------------------------------------------------
/**
* XOR arrays. Postcondition out[i] = in[i] ^ in2[i] forall i = 0...length
* @param out the output buffer
* @param in the first input buffer
* @param in2 the second output buffer
* @param length the length of the three buffers
*/
inline void xorBuf(unsigned char*out, const unsigned char*in1, const unsigned char*in2, size_t len){
    while(len >= 16){
        //ToDo: try cast to uint64
        uint64_t x0, x1, y0, y1;
        memcpy(&x0, in1     , 8);
        memcpy(&x1, in1+8   , 8);
        memcpy(&y0, in2     , 8);
        memcpy(&y1, in2+8   , 8);

        x0 ^= y0;
        x1 ^= y1;
        memcpy(out  , &x0, 8);
        memcpy(out+8, &x1, 8);
        out += 16;
        in1 += 16;
        in2 += 16;
        len -= 16;
    }

   for(int i=int(len); i-->0; out[i] = in1[i] ^ in2[i]);
}

//--------------------------------------------------------------------------------
//Combine cipher stream with message
void chacha(const uint8_t*inp, uint8_t*out, size_t len){
   //verify_key_set(m_state.empty() == false);
#if 0
   while(len >= m_buffer.size()-m_position){
      xorBuf(out, inp, &m_buffer[m_position], m_buffer.size() - m_position);
      len -= (m_buffer.size() - m_position);
      inp += (m_buffer.size() - m_position);
      out += (m_buffer.size() - m_position);
      chacha_x4(m_buffer.data(), m_state.data(), m_rounds);
      m_position = 0;
      }

   xorBuf(out, inp, &m_buffer[m_position], len);

   m_position += length;
#endif
}


//--------------------------------------------------------------------------------
ChaCha::ChaCha(){
}

//--------------------------------------------------------------------------------
void ChaCha::setKey(const uint8_t*key, size_t len){
}

//--------------------------------------------------------------------------------
void ChaCha::setIv(const uint8_t*iv, size_t len){
}

//--------------------------------------------------------------------------------
void ChaCha::operator()(uint8_t*buf, size_t len){
}
