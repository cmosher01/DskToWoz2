# DskToWoz2

Copyright © 2019, Christopher Alan Mosher, Shelton, Connecticut, USA, <cmosher01@gmail.com>.

[![License](https://img.shields.io/github/license/cmosher01/DskToWoz2.svg)](https://www.gnu.org/licenses/gpl.html)
[![Latest Release](https://img.shields.io/github/release-pre/cmosher01/DskToWoz2.svg)](https://github.com/cmosher01/DskToWoz2/releases/latest)
[![Build Status](https://travis-ci.com/cmosher01/DskToWoz2.svg?branch=master)](https://travis-ci.com/cmosher01/DskToWoz2)
[![Build status](https://ci.appveyor.com/api/projects/status/923ao28tl8ht7t1c?svg=true)](https://ci.appveyor.com/project/cmosher01/dsktowoz2)
[![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=CVSSQ2BWDCKQ2)

Converts Apple ][ disk images in
[DSK format](http://apple2.org.za/gswv/a2zine/faqs/Csa2FLUTILS.html#006) to
[WOZ 2.0 format](https://applesaucefdc.com/woz/reference2/).

`*.dsk` → `*.woz`

---

Input floppy disk images are expected to be in Apple DOS 3.x format (16-sector or 13-sector),
with one physical byte per logical byte. Common file types include: `.dsk` `.do` `.d13` `.d16`

16-sector image files must be 143360 bytes in size, and 13-sector images must be 116480 bytes in size.
Images must be normal DOS 3.x disks with a normal VTOC at track $11.

ProDOS (PO) order is not supported. Nibble NIB disk images, or any other formats, are not supported.

Existing `.woz` files will not be overwritten.

The source code is in C and C++, and depends on [Qt](https://doc.qt.io/qt-5/gettingstarted.html).
