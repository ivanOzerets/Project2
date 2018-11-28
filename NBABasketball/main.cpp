#include "mainwindow.h"
#include "login.h"
#include "teamselection.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    login l;
    teamSelection ts;

    ts.show();  //displays team selection window upon execution
    //l.show(); //displays login upon execution
    //w.show(); //displays main window upon execution

    return a.exec();
}
