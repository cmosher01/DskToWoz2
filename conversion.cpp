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

Conversion::Conversion(const QFileInfo &filePath, const QFileInfo &root):
    _root(abs(root)),
    _dsk(abs(filePath)),
    _woz(abs(filePath.filePath()+".woz")),
    _dos(readDos(_dsk)) {
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
