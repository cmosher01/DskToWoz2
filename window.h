#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QTableView>
#include <QStandardItemModel>
#include <QDir>

class Window : public QWidget {
    Q_OBJECT

    private:
        QComboBox *fileComboBox = nullptr;
        QComboBox *directoryComboBox = nullptr;
        QLabel *filesFoundLabel = nullptr;
        QPushButton *findButton = nullptr;
        QTableView *filesTable = nullptr;
        QStandardItemModel *model = nullptr;
        QDir *currentDir = nullptr;

        void createFilesTable();
        QStandardItem *colFile(const QString &filePath);
        QStandardItem *colSize(const QString &filePath);
        QStandardItem *colVtoc(const QString &filePath);
        QStandardItem *colFileWoz(const QString &filePath);
        QStandardItem *colDir(const QString &filePath);

    private slots:
        void browse();
        void find();
        void animateFindClick();

    public:
        Window(QWidget *parent = nullptr);
        virtual ~Window();
};

#endif
