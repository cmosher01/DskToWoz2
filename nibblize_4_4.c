#include "nibblize_4_4.h"
#include <stdint.h>

static const uint16_t MASK = 0xAAAAu;

uint16_t nibblize_4_4_encode(uint8_t n)
{
  /*
    hgfedcba00000000   n<<8
    000000000hgfedcb   n>>1
  | 1010101010101010   0xAAAA
  ------------------
    1g1e1c1a1h1f1d1b
  */
  return (uint16_t)((n << 8) | (n >> 1) | MASK);
}

uint8_t nibblize_4_4_decode(uint16_t n)
{
  /*
    g1e1c1a1h1f1d1b1   n<<1|1
  & 000000001g1e1c1a   n>>8
    ------------------
    00000000hgfedcba
  */
  return ((n << 1) | 1) & (n >> 8);
}
