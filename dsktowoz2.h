#ifndef DSKTOWOZ2_H
#define DSKTOWOZ2_H

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif



extern void dskToWoz2(const uint8_t *rb_dsk, const char *name_dsk, uint32_t crc_dsk, int sector16, const char *path_woz);



#ifdef __cplusplus
}
#endif

#endif // DSKTOWOZ2_H
