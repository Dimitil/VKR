#include "mainwindow.h"

#include <QDebug>
#include <QApplication>
#include <model.h>

int main(int argc, char *argv[])
{

    Model M;
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //qDebug() << M.get()[2][3];
    return a.exec();
}
