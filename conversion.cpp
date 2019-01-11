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
#include "conversion.h"
#include <QString>
#include <QFile>
#include <QDir>
#include <QFileInfo>

static QFileInfo abs(const QFileInfo &f) {
    QFileInfo t(f);
    t.makeAbsolute();
    return t;
}

static QString readDos(const QFileInfo &dsk) {
    const qint64 size = dsk.size();

    const bool d16 = (size == 0x23000u);
    const bool d13 = (size == 0x1C700u);
    if (!(d16 || d13)) {
        return "";
    }

    QFile f(dsk.filePath());
    if (!f.open(QIODevice::ReadOnly)) {
        return "";
    }



    if (!f.seek(d16 ? 0x11001u : 0x0DD01u)) {
        return "";
    }

    char b = 0;
    f.read(&b, 1);
    if (b != 0x11) {
        return "";
    }



    if (!f.seek(d16 ? 0x11027 : 0x0DD27)) {
        return "";
    }

    b = 0;
    f.read(&b, 1);
    if (b != 0x7A) {
        return "";
    }



    f.seek(d16 ? 0x11003u : 0x0DD03u);
    b = 0;
    f.read(&b, 1);
    b += '0';
    if (b < '0' || '9' < b) {
        b = '?';
    }
    return "DOS 3." + QString(b);
}

static bool calc13(const QFileInfo &dsk) {
    const qint64 size = dsk.size();
    return size == 0x1C700u;
}

Conversion::Conversion(const QFileInfo &filePath, const QFileInfo &root):
    _root(abs(root)),
    _dsk(abs(filePath)),
    _woz(abs(filePath.filePath()+".woz")),
    _dos(readDos(_dsk)),
    _13(calc13(_dsk)) {
}

QFileInfo Conversion::dsk() const {
    return this->_dsk;
}

QFileInfo Conversion::woz() const {
    return this->_woz;
}

QFileInfo Conversion::root() const {
    return this->_root;
}

QString Conversion::dos() const {
    return this->_dos;
}

bool Conversion::ok() const {
    return !this->_dos.isEmpty() && !this->_woz.exists();
}

bool Conversion::is13() const {
    return this->_13;
}
