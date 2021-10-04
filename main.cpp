#include "mainwindow.h"

#include <QApplication>
#include "singleapplication.h"

int main(int argc, char *argv[])
{
    SingleApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}
