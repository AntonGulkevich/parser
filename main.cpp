#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    QString ptf (*argv);
    ptf +="commod.bin";
    //w.setPathToFolder(ptf);
    //w.setAutoMode(false); // 0-single 1-multi
    w.show();

    return a.exec();
}
