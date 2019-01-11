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
#include "nibblize_5_3_alt.h"
#include "nibblize_5_3_common.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>



static void nibblize(const uint8_t **pdata, uint8_t **encoded)
{
  const uint8_t *data = *pdata;
  uint8_t *enc = *encoded;
  int i;
  int base = 0;
  for (i = 0; i < GRP; ++i)
    {
      enc[base + i]  = (data[base    + i] & 0x07) << 2;
      enc[base + i] |= (data[3 * GRP + i] & 0x04) >> 1;
      enc[base + i] |= (data[4 * GRP + i] & 0x04) >> 2;
    }
  base += GRP;
  for (i = 0; i < GRP; ++i)
    {
      enc[base + i]  = (data[base    + i] & 0x07) << 2;
      enc[base + i] |= (data[3 * GRP + i] & 0x02);
      enc[base + i] |= (data[4 * GRP + i] & 0x02) >> 1;
    }
  base += GRP;
  for (i = 0; i < GRP; ++i)
    {
      enc[base + i]  = (data[base    + i] & 0x07) << 2;
      enc[base + i] |= (data[3 * GRP + i] & 0x01) << 1;
      enc[base + i] |= (data[4 * GRP + i] & 0x01);
    }
  base += GRP;
  enc[base] = 0;
  ++base;
  for (i = 0; i < 5 * GRP; ++i)
    {
      enc[base + i] = data[i] >> 3;
    }
  base += 5 * GRP;
  enc[base] = data[5 * GRP] & 0x1F; /* throw out high 3 bits */

  (*pdata) += 0x100;
}

static void flipBuf2(uint8_t *enc)
{
  int i;
  int sw = BUF2_SIZ;
  for (i = 0; i < BUF2_SIZ / 2; ++i)
    {
      int tmp;
      --sw;
      tmp = enc[i];
      enc[i] = enc[sw];
      enc[sw] = tmp;
    }
}

static void xorBuf(uint8_t **encoded)
{
  uint8_t *enc = *encoded;
  int i;
  enc[BUF1_SIZ+BUF2_SIZ] = 0;
  for (i = BUF1_SIZ+BUF2_SIZ; i > 0; --i)
    {
      enc[i] ^= enc[i - 1];
    }
}

static void xlateBuf(uint8_t **encoded)
{
  int i;
  for (i = 0; i < BUF1_SIZ+BUF2_SIZ+1; ++i)
    {
      **encoded = xlate[**encoded];
      (*encoded)++;
    }
}

void nibblize_5_3_alt_encode(const uint8_t **original, uint8_t **encoded)
{
  nibblize(original, encoded);
  flipBuf2(*encoded);
  xorBuf(encoded);
  xlateBuf(encoded);
}


static void ulateBuf(uint8_t *enc)
{
  int i;
  for (i = 0; i < BUF1_SIZ+BUF2_SIZ+1; ++i)
    {
      enc[i] = ulate[enc[i]];
    }
}

static void unxorBuf(uint8_t *enc)
{
  int i;
  for (i = 1; i < BUF1_SIZ+BUF2_SIZ+1; ++i)
    {
      enc[i] ^= enc[i - 1];
    }
}

static void denibblize(uint8_t *buf, uint8_t **decoded)
{
  uint8_t *data = *decoded;
  int bufbase = 3 * GRP + 1;
  int base;
  int i;

  for (i = 0; i < 5 * GRP; ++i)
    {
      data[i] = buf[bufbase + i] << 3;
    }
  data[5 * GRP] = buf[bufbase + 5 * GRP];

  base = 0;
  for (i = 0; i < GRP; ++i)
    {
      data[base + i] |= buf[base + i] >> 2;
      data[3 * GRP + i] |= (buf[base + i] & 0x02) << 1;
      data[4 * GRP + i] |= (buf[base + i] & 0x01) << 2;
    }

  base += GRP;
  for (i = 0; i < GRP; ++i)
    {
      data[base + i] |= buf[base + i] >> 2;
      data[3 * GRP + i] |= (buf[base + i] & 0x02);
      data[4 * GRP + i] |= (buf[base + i] & 0x01) << 1;
    }

  base += GRP;
  for (i = 0; i < GRP; ++i)
    {
      data[base + i] |= buf[base + i] >> 2;
      data[3 * GRP + i] |= (buf[base + i] & 0x02) >> 1;
      data[4 * GRP + i] |= (buf[base + i] & 0x01);
    }

  (*decoded) += 0x100;
}

void nibblize_5_3_alt_decode(const uint8_t **original, uint8_t **decoded)
{
  uint8_t buf[BUF1_SIZ+BUF2_SIZ+1];
  build_ulate_table();
  memcpy(buf,*original,sizeof(buf));
  (*original) += sizeof(buf);
  ulateBuf(buf);
  unxorBuf(buf);
  flipBuf2(buf);
  denibblize(buf, decoded);
}
