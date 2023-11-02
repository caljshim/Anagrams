#include "mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QApplication::setQuitOnLastWindowClosed(false);
    w.show();
    return a.exec();
}
