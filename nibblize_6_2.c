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
#include "nibblize_6_2.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>



#define GRP 0x56
#define BUF1_SIZ 0x0100
#define BUF2_SIZ GRP

static const uint8_t xlate[] =
{
                          0x96, 0x97,       0x9A, 0x9B,       0x9D, 0x9E, 0x9F,
                          0xA6, 0xA7,     /*0xAA*/0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
  0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
                                                  0xCB,       0xCD, 0xCE, 0xCF,
        0xD3,     /*0xD5*/0xD6, 0xD7, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
                    0xE5, 0xE6, 0xE7, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
  0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF
};

static uint8_t ulate[1<<sizeof(uint8_t)];

static void build_ulate_table(void)
{
  uint_fast8_t i;
  memset(ulate,0xFF,sizeof(ulate));
  for (i = 0; i < sizeof(xlate)/sizeof(xlate[0]); ++i)
    {
      ulate[xlate[i]] = i;
    }
}



static void buildBuffers(const uint8_t **data, uint8_t *top, uint8_t *two)
{
  int i;
  int j = BUF2_SIZ - 1;
  int twoShift = 0;
  memset(two,0,BUF2_SIZ);
  for (i = 0; i < BUF1_SIZ; ++i)
    {
      uint_fast32_t val = *(*data)++;
      top[i] = val >> 2;
      two[j] |= ((val & 0x01) << 1 | (val & 0x02) >> 1) << twoShift;
      if (j <= 0)
        {
          j = BUF2_SIZ;
          twoShift += 2;
        }
      --j;
    }
}

/**
 * Encodes the given sector (256 bytes) using the Apple ][ "6 and 2" encoding
 * scheme. Based on code by Andy McFadden, from CiderPress.
 *
 * @param data
 *          sector to encode
 * @return encoded sector (343 bytes)
 */
void nibblize_6_2_encode(const uint8_t **original, uint8_t **encoded)
{
  uint8_t top[BUF1_SIZ];
  uint8_t two[BUF2_SIZ];
  int chksum = 0;
  int i;
  buildBuffers(original, top, two);
  for (i = BUF2_SIZ - 1; i >= 0; --i)
    {
      *(*encoded)++ = xlate[two[i] ^ chksum];
      chksum = two[i];
    }
  for (i = 0; i < BUF1_SIZ; ++i)
    {
      *(*encoded)++ = xlate[top[i] ^ chksum];
      chksum = top[i];
    }
  *(*encoded)++ = xlate[chksum];
}

/**
 * Decodes the given sector (256 logical bytes, 343 actual bytes) using the
 * Apple ][ "6 and 2" encoding scheme. Based on code by Andy McFadden, from
 * CiderPress.
 *
 * @param enc
 *          encoded sector
 * @return sector (256 bytes) (decoded)
 * @throws CorruptDataException
 *           if the checksum in the given encoded data is not correct
 */
void nibblize_6_2_decode(const uint8_t **original, uint8_t **decoded)
{
  uint8_t two[3 * GRP];
  int chksum = 0;
  uint8_t decodedVal;
  int i;

  build_ulate_table();

  /*
   * Pull the 342 bytes out, convert them from disk bytes to 6-bit values, and
   * arrange them into a DOS-like pair of buffers.
   */
  for (i = 0; i < GRP; ++i)
    {
      decodedVal = ulate[*(*original)++];
      /* TODO handle invalid nibble value
      if (decodedVal == 0xFF) {
      }
      */
      chksum ^= decodedVal;
      two[i + 0 * GRP] = ((chksum & 0x01) << 1) | ((chksum & 0x02) >> 1);
      two[i + 1 * GRP] = ((chksum & 0x04) >> 1) | ((chksum & 0x08) >> 3);
      two[i + 2 * GRP] = ((chksum & 0x10) >> 3) | ((chksum & 0x20) >> 5);
    }
  for (i = 0; i < 0x100; ++i)
    {
      decodedVal = ulate[*(*original)++];
      /* TODO handle invalid nibble value
      if (decodedVal == 0xFF) {
      }
      */
      chksum ^= decodedVal;
      *(*decoded)++ = (chksum << 2) | two[i];
    }
  /*
   * Grab the 343rd byte (the checksum byte) and see if we did this right.
   */
  decodedVal = ulate[*(*original)++];
  /* TODO handle invalid nibble value
  if (decodedVal == 0xFF) {
  }
  */
  chksum ^= decodedVal;
  /* TODO handle bad checksum value
  if (chksum != 0) {
  }
  */
}
