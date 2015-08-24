#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    if (argc>1){
        (char (*argv[1])== '0' ) ? w.setAutoMode(false) : w.setAutoMode(true);
        QString ptf(argv[2]);
        w.setPathToFolder(ptf);
        w.autoStart();
    }
    else
        w.show();

    return a.exec();
}
