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
