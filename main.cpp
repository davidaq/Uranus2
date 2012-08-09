#include <QtGui/QApplication>
#include "mainwindow.h"
#include "cfg.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Cfg::pauseAlarm(false);

    MainWindow w;
    w.show();
    
    return a.exec();
}
