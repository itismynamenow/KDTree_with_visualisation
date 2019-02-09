#include <QApplication>
#include <QWidget>

#include <vector>

#include "mainwindow.h"

using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
