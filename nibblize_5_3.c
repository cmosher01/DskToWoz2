/*
    DskToWoz2

    Copyright © 2019, Christopher Alan Mosher, Shelton, CT, USA. <cmosher01@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY, without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
#include "nibblize_5_3.h"
#include "nibblize_5_3_common.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>



/*
 * Encodes the given sector (256 bytes) using the normal "5 and 3" encoding
 * scheme. Based on code by Andy McFadden, from CiderPress.
 *
 * original sector to encode;  pointer updated on exit
 * encoded  encoded sector; pointer updated on exit
 */
void nibblize_5_3_encode(const uint8_t **original, uint8_t **encoded)
{
  uint8_t top[BUF1_SIZ];
  uint8_t thr[BUF2_SIZ];
  uint8_t chksum = 0;
  int i, val;

  /* Split the bytes into sections. */
  for (i = GRP - 1; i >= 0; --i)
    {
      uint8_t three1,three2,three3,three4,three5;

      three1 = *(*original)++;
      top[i + 0 * GRP] = three1 >> 3;
      three2 = *(*original)++;
      top[i + 1 * GRP] = three2 >> 3;
      three3 = *(*original)++;
      top[i + 2 * GRP] = three3 >> 3;
      three4 = *(*original)++;
      top[i + 3 * GRP] = three4 >> 3;
      three5 = *(*original)++;
      top[i + 4 * GRP] = three5 >> 3;

      thr[i + 0 * GRP] = (three1 & 0x07) << 2 | (three4 & 0x04) >> 1 | (three5 & 0x04) >> 2;
      thr[i + 1 * GRP] = (three2 & 0x07) << 2 | (three4 & 0x02)      | (three5 & 0x02) >> 1;
      thr[i + 2 * GRP] = (three3 & 0x07) << 2 | (three4 & 0x01) << 1 | (three5 & 0x01);
    }

  /* Handle the last byte. */
  val = *(*original)++;
  top[5 * GRP] = val >> 3;
  thr[3 * GRP] = val & 0x07;

  /* Write the bytes. */
  for (i = BUF2_SIZ - 1; i >= 0; --i)
    {
      *(*encoded)++ = xlate[thr[i] ^ chksum];
      chksum = thr[i];
    }
  for (i = 0; i < BUF1_SIZ; ++i)
    {
      *(*encoded)++ = xlate[top[i] ^ chksum];
      chksum = top[i];
    }
  *(*encoded)++ = xlate[chksum];
}

static uint8_t unxlate(uint8_t b)
{
  uint8_t decodedVal = ulate[b];
  if (decodedVal == 0xFF)
    {
      /* TODO handle invalid nibble value */
    }
  return decodedVal;
}


void nibblize_5_3_decode(const uint8_t **original, uint8_t **decoded)
{
  uint8_t top[BUF1_SIZ];
  uint8_t thr[BUF2_SIZ];
  uint8_t chksum = 0;
  uint8_t val;
  int i;

  build_ulate_table();

  /*
   * Pull the 410 bytes out, convert them from disk bytes to 5-bit values, and
   * arrange them into a DOS-like pair of buffers.
   */
  for (i = BUF2_SIZ - 1; i >= 0; --i)
    {
      val = unxlate(*(*original)++);
      chksum ^= val;
      thr[i] = chksum;
    }
  for (i = 0; i < BUF1_SIZ; ++i)
    {
      val = unxlate(*(*original)++);
      chksum ^= val;
      top[i] = chksum << 3;
    }

  /*
   * Grab the 411th byte (the checksum byte) and see if we did this right.
   */
  val = unxlate(*(*original)++);
  chksum ^= val;
  if (chksum)
    {
      /* TODO handle bad checksum */
    }

  /* Convert this pile of stuff into 256 data bytes. */
  for (i = GRP - 1; i >= 0; --i)
    {
      uint8_t three1,three2,three3,three4,three5;

      three1 = thr[0 * GRP + i];
      *(*decoded)++ = top[0 * GRP + i] | ((three1 >> 2) & 0x07);
      three2 = thr[1 * GRP + i];
      *(*decoded)++ = top[1 * GRP + i] | ((three2 >> 2) & 0x07);
      three3 = thr[2 * GRP + i];
      *(*decoded)++ = top[2 * GRP + i] | ((three3 >> 2) & 0x07);

      three4 = (three1 & 0x02) << 1 | (three2 & 0x02)      | (three3 & 0x02) >> 1;
      *(*decoded)++ = top[3 * GRP + i] | ((three4     ) & 0x07);
      three5 = (three1 & 0x01) << 2 | (three2 & 0x01) << 1 | (three3 & 0x01);
      *(*decoded)++ = top[4 * GRP + i] | ((three5     ) & 0x07);
    }

  /* Convert the very last byte, which is handled specially. */
  *(*decoded)++ = top[5 * GRP] | (thr[3 * GRP] & 0x07);
}
