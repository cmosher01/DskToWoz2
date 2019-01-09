#include "conversion.h"
#include <QString>
#include <QFile>
#include <QFileInfo>

Conversion::Conversion(const QString &filePath) {
    const QFileInfo info(this->currentDir->relativeFilePath(filePath));
}
