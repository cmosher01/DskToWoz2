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
#include "nibblize_5_3_common.h"
#include <stdint.h>
#include <string.h>



const uint8_t xlate[] = {
                  /*0xAA*/0xAB, 0xAD, 0xAE, 0xAF,
  0xB5, 0xB6, 0xB7, 0xBA, 0xBB, 0xBD, 0xBE, 0xBF,
/*0xD5*/0xD6, 0xD7, 0xDA, 0xDB, 0xDD, 0xDE, 0xDF,
                    0xEA, 0xEB, 0xED, 0xEE, 0xEF,
  0xF5, 0xF6, 0xF7, 0xFA, 0xFB, 0xFD, 0xFE, 0xFF,
};

uint8_t ulate[1<<sizeof(uint8_t)];

void build_ulate_table(void) {
    uint_fast8_t i;
    memset(ulate,0xFF,sizeof(ulate));
    for (i = 0; i < sizeof(xlate)/sizeof(xlate[0]); ++i) {
        ulate[xlate[i]] = i;
    }
}
