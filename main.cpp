#include "mainwindow.h"
#include <QDialog>
#include <QApplication>
#include <QLabel>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    MainWindow w;
    QObject::connect( &w, &MainWindow::quit, &a, &QApplication::quit);
    w._main_menu->exec();

    //w.show();

    return a.exec();
}
