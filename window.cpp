#include "window.h"
#include <QApplication>
#include <QShortcut>
#include <QLineEdit>
#include <QGridLayout>
#include <QFileDialog>
#include <QProgressDialog>
#include <QDirIterator>
#include <QHeaderView>

enum {
    absoluteFileNameRole = Qt::UserRole + 1
};

static QComboBox *createComboBox(const QString &text) {
    auto *const comboBox = new QComboBox();
    comboBox->setEditable(true);
    comboBox->addItem(text);
    comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    return comboBox;
}

Window::Window(QWidget *parent)
    : QWidget(parent) {
    connect(new QShortcut(QKeySequence::Quit, this), &QShortcut::activated, QCoreApplication::instance(), &QApplication::quit);

    setWindowTitle(tr("DskToWoz2"));

    QPushButton *browseButton = new QPushButton(tr("\u2190 &Browse"), this);
    connect(browseButton, &QAbstractButton::clicked, this, &Window::browse);

    this->findButton = new QPushButton(tr("&Find"), this);
    this->findButton->setFocus();
    connect(this->findButton, &QAbstractButton::clicked, this, &Window::find);

    fileComboBox = createComboBox(tr("*.dsk; *.do; *.d16; *.d13"));
    connect(fileComboBox->lineEdit(), &QLineEdit::returnPressed, this, &Window::animateFindClick);
    directoryComboBox = createComboBox(QDir::toNativeSeparators(QDir::currentPath()));
    connect(directoryComboBox->lineEdit(), &QLineEdit::returnPressed, this, &Window::animateFindClick);

    filesFoundLabel = new QLabel;

    createFilesTable();

    auto *const mainLayout = new QGridLayout(this);
    mainLayout->addWidget(new QLabel(tr("File name pattern:")), 0, 0);
    mainLayout->addWidget(fileComboBox, 0, 1, 1, 2);

    mainLayout->addWidget(new QLabel(tr("Directory:")), 2, 0);
    mainLayout->addWidget(directoryComboBox, 2, 1);
    mainLayout->addWidget(browseButton, 2, 2);

    mainLayout->addWidget(findButton, 3, 0);
    mainLayout->addWidget(filesTable, 4, 0, 1, 3);
    mainLayout->addWidget(filesFoundLabel, 5, 0, 1, 3);

    setWindowState(Qt::WindowMaximized);
    show();
}

Window::~Window() {
    delete this->filesTable;
    delete this->model;
}

void Window::browse() {
    QString directory = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("Root directory"), QDir::currentPath()));
    if (!directory.isEmpty()) {
        if (directoryComboBox->findText(directory) == -1) {
            directoryComboBox->addItem(directory);
        }
        directoryComboBox->setCurrentIndex(directoryComboBox->findText(directory));
    }
}

static void updateComboBox(QComboBox *comboBox) {
    if (comboBox->findText(comboBox->currentText()) == -1) {
        comboBox->addItem(comboBox->currentText());
    }
}

QStandardItem *Window::colFile(const QString &filePath) {
    const QFileInfo info(filePath);
    return new QStandardItem(info.fileName());
}

QStandardItem *Window::colDir(const QString &filePath) {
    const QFileInfo info(this->currentDir->relativeFilePath(filePath));
    return new QStandardItem(info.path()+QDir::separator());
}

QStandardItem *Window::colFileWoz(const QString &filePath) {
    const QFileInfo info(filePath);

    const bool ok = !info.exists();
    const QIcon &icon = QApplication::style()->standardIcon(
        ok
        ? QStyle::SP_DialogApplyButton
        : QStyle::SP_DialogCancelButton);

    return new QStandardItem(icon, info.fileName()+(ok ? "" : " (exists)"));
}

QStandardItem *Window::colVtoc(const QString &filePath) {
    const qint64 size = QFileInfo(filePath).size();

    const bool d16 = (size == 0x23000u);
    const bool d13 = (size == 0x1C700u);
    if (!(d16 || d13)) {
        return new QStandardItem(QApplication::style()->standardIcon(QStyle::SP_DialogCancelButton), "");
    }

    QFile f(filePath);
    if (!f.open(QIODevice::ReadOnly)) {
        return new QStandardItem(QApplication::style()->standardIcon(QStyle::SP_DialogCancelButton), "");
    }



    if (!f.seek(d16 ? 0x11001u : 0x0DD01u)) {
        return new QStandardItem(QApplication::style()->standardIcon(QStyle::SP_DialogCancelButton), "");
    }

    char b = 0;
    f.read(&b, 1);
    if (b != 0x11) {
        return new QStandardItem(QApplication::style()->standardIcon(QStyle::SP_DialogCancelButton), "");
    }



    if (!f.seek(d16 ? 0x11027 : 0x0DD27)) {
        return new QStandardItem(QApplication::style()->standardIcon(QStyle::SP_DialogCancelButton), "");
    }

    b = 0;
    f.read(&b, 1);
    if (b != 0x7A) {
        return new QStandardItem(QApplication::style()->standardIcon(QStyle::SP_DialogCancelButton), "");
    }

    f.seek(d16 ? 0x11003u : 0x0DD03u);
    b = 0;
    f.read(&b, 1);
    b += '0';
    if (b < '0' || '9' < b) {
        b = '?';
    }
    QString dos = "DOS 3.";
    dos += b;
    return new QStandardItem(QApplication::style()->standardIcon(QStyle::SP_DialogApplyButton), dos);
}

QStandardItem *Window::colSize(const QString &filePath) {
    const qint64 size = QFileInfo(filePath).size();
    char hexbuf[32];
    sprintf(hexbuf, "$%llX", size);

    const bool ok = (size == 0x23000u || size == 0x1C700u);
    const QIcon &icon = QApplication::style()->standardIcon(
        ok
        ? QStyle::SP_DialogApplyButton
        : QStyle::SP_DialogCancelButton);

    return new QStandardItem(icon, hexbuf);
}

void Window::find() {
    QString fileName = this->fileComboBox->currentText();
    QString path = QDir::cleanPath(this->directoryComboBox->currentText());
    this->currentDir = new QDir(path);

    updateComboBox(this->fileComboBox);
    updateComboBox(this->directoryComboBox);

    QProgressDialog progressDialog(tr("Locating disk images..."), tr("&Cancel"), 0, 0, this, nullptr);
    progressDialog.setWindowModality(Qt::WindowModal);
    progressDialog.setWindowTitle(tr("Find Files"));
    progressDialog.show();

    delete this->model;
    this->model = new QStandardItemModel();
    QStringList header;
    header << tr("Source DSK file") << tr("Size") << tr("VTOC") << tr("Destination WOZ file") << tr("Directory");
    this->model->setHorizontalHeaderLabels(header);
    this->filesTable->setModel(this->model);

    QStringList filter = fileName.split(";");
    filter.replaceInStrings(QRegularExpression("^ *(.*) *$"), "\\1");
    QDirIterator it(path, filter, QDir::AllEntries | QDir::Readable | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    std::int32_t c = 0;
    while (it.hasNext() && !progressDialog.wasCanceled()) {
        const QString filePath = it.next();
        ++c;



        QList<QStandardItem*> items;

        items.append(colFile(filePath));
        items.append(colSize(filePath));
        items.append(colVtoc(filePath));
        items.append(colFileWoz(filePath+".woz"));
        items.append(colDir(filePath));

        this->model->appendRow(items);

        this->filesTable->setRowHeight(c-1, 8);
        this->filesTable->scrollToBottom();


        this->filesFoundLabel->setText(tr("%n file(s) found. (Search in progress.)", "", c));
        progressDialog.setLabelText(tr("%n file(s) found", "", c));
        QCoreApplication::processEvents();
    }
    const int cols = this->model->columnCount();
    for (int i = 1; i < cols; ++i) {
        this->filesTable->resizeColumnToContents(i);
    }

    if (progressDialog.wasCanceled()) {
        this->filesFoundLabel->setText(tr("%n file(s) found. (Search was cancelled.)", "", c));
    } else {
        this->filesFoundLabel->setText(tr("%n file(s) found. (Search complete.)", "", c));
    }
}

void Window::animateFindClick() {
    this->findButton->animateClick();
}

void Window::createFilesTable() {
    delete this->filesTable;
    this->filesTable = new QTableView(this);
    this->filesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->filesTable->horizontalHeader()->setStretchLastSection(true);
    this->filesTable->setAlternatingRowColors(true);
    this->filesTable->setShowGrid(false);
}
