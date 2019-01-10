#ifndef UUID_38d0bc0d96ef449e856681a7a5bebc74
#define UUID_38d0bc0d96ef449e856681a7a5bebc74

#include <stdint.h>

#ifndef GRP
#define GRP 0x33
#endif

#ifndef BUF1_SIZ
#define BUF1_SIZ 0x0100
#endif

#ifndef BUF2_SIZ
#define BUF2_SIZ (3 * GRP + 1)
#endif

extern const uint8_t xlate[];
extern uint8_t ulate[];

extern void build_ulate_table(void);

#endif
