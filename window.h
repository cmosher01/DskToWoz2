#ifndef WINDOW_H
#define WINDOW_H

#include "conversion.h"
#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QTableView>
#include <QStandardItemModel>
#include <QDir>
#include <QLinkedList>

class Window : public QWidget {
    Q_OBJECT

    private:
        QComboBox *fileComboBox = nullptr;
        QComboBox *directoryComboBox = nullptr;
        QFileInfo dirRoot;
        QLabel *filesFoundLabel = nullptr;
        QTableView *filesTable = nullptr;
        QStandardItemModel *model = nullptr;
        QPushButton *convertButton = nullptr;

        QLinkedList<const Conversion> cvts;

        void createFilesTable();

    private slots:
        void browse();
        void find();
        void convert();

    public:
        Window(QWidget *parent = nullptr);
        virtual ~Window();
};

#endif
