#ifndef CONVERSION_H
#define CONVERSION_H

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QString>

class Conversion final {
    private:
        const QFileInfo _root;
        const QFileInfo _dsk;
        const QFileInfo _woz;
        const QString _dos;
        const bool _13;

    public:
        Conversion(const QFileInfo &filePath, const QFileInfo &root);
        QFileInfo dsk() const;
        QFileInfo woz() const;
        QFileInfo root() const;
        QString dos() const;
        bool ok() const;
        bool is13() const;
};

#endif // CONVERSION_H
