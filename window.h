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
