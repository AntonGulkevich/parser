#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFrame>
#include <QPoint>
#include <QApplication>
#include <QDesktopWidget>
#include <QPushButton>
#include <QProgressBar>
#include <QFile>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDebug>
#include <QAbstractAnimation>
#include <QString>


class MainWindow : public QFrame
{
    Q_OBJECT
private:
    QPoint mainWindownStartPoint;
    QSize mainWindowSize;

    QPushButton *openFileButton;
    QPushButton *openDirButton;
    QPushButton *closeButton;
    QPushButton *optionsButton;
    QPushButton *log;

    QFileDialog *openDirDlg;
    QFileDialog *openFileDlg;

    QString pathToFolder;

    void initDefaultStyle();
    void initContent();

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void onCloseButtonClicked();
    bool onOpenFileButtonClicked();
    bool onOpenDirButtonClicked();
};

#endif // MAINWINDOW_H
