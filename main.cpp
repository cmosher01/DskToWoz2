#include <QApplication>
#include <QDebug>
#include "window.h"

extern "C" { extern void x(); }

int main(int argc, char *argv[]) {
    qDebug() << "STARTING";
    QApplication app(argc, argv);
    qDebug() << "creating Window";
    Window window;
    qDebug() << "exec...";
    return app.exec();
}
