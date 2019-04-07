# DskToWoz2



Copyright © 2019, Christopher Alan Mosher, Shelton, CT, USA <cmosher01@gmail.com>



[![Latest Release](https://img.shields.io/github/release-pre/cmosher01/DskToWoz2.svg)](https://github.com/cmosher01/DskToWoz2/releases/latest)
[![License](https://img.shields.io/github/license/cmosher01/DskToWoz2.svg)](https://www.gnu.org/licenses/gpl.html)
[![Build Status](https://travis-ci.com/cmosher01/DskToWoz2.svg?branch=master)](https://travis-ci.com/cmosher01/DskToWoz2)



Converts Apple ][ disk images in DSK format to [WOZ 2.0 format](https://applesaucefdc.com/woz/reference2/) .



Input floppy disk images are expected to be in Apple DOS 3.x (16-sector or 13-sector)
format, with one physical byte per logical byte. Common file types are .DSK, .DO, .D13, or .D16.

16-sector image files must be 143360 bytes in size, and 13-sector images must be 116480 bytes in size.
Images must be normal DOS 3.x disks with a normal VTOC at track $11.

ProDOS (PO) order is not supported. Nibble NIB disk images, or any other format, are not supported.

Existing WOZ files will not be overwritten.

The source code is in C and C++, and depends on Qt.
