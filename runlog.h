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
#ifndef RUNLOG_H
#define RUNLOG_H

#include <QObject>
#include <QDialog>
#include <QPlainTextEdit>
#include <QString>
#include <QPushButton>

class RunLog : public QDialog {
    Q_OBJECT

    private:
        QPlainTextEdit *_log;
        QPushButton *_quit;

    private slots:
        void quitApp();

    public:
        explicit RunLog(QWidget *parent = nullptr);
        void log(const QString &msg);
        void end();
};

#endif // RUNLOG_H
