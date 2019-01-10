#ifndef UUID_30af0ca9a8a74a59a85b2e33670ed1ce
#define UUID_30af0ca9a8a74a59a85b2e33670ed1ce

#include <stdint.h>

void nibblize_6_2_encode(const uint8_t **original, uint8_t **encoded);
void nibblize_6_2_decode(const uint8_t **original, uint8_t **decoded);

#endif
