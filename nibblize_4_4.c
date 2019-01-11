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
