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
#include "runlog.h"
#include <QDialog>
#include <QWidget>
#include <QVBoxLayout>
#include <QPlainTextEdit>
#include <QPushButton>

RunLog::RunLog(QWidget *parent):
    QDialog(parent) {
    auto *const mainLayout = new QVBoxLayout();
    this->_log = new QPlainTextEdit();
    this->_log->setFont(QFont("Helvetica", 8));
    mainLayout->addWidget(this->_log);
    this->_quit = new QPushButton(tr("Quit"));
    this->_quit->setEnabled(false);
    connect(this->_quit, &QAbstractButton::clicked, this, &RunLog::quitApp);
    mainLayout->addWidget(this->_quit);
    setLayout(mainLayout);
    setWindowState(Qt::WindowMaximized);
    show();
}

void RunLog::quitApp() {
    close();
}

void RunLog::log(const QString &msg) {
    this->_log->appendPlainText(msg);
}

void RunLog::end() {
    this->_quit->setEnabled(true);
    this->_quit->setFocus();
}
