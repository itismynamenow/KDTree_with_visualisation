#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "kdtreevisualisationwidget.h"

using namespace std;

class MainWindow: public QWidget
{
    Q_OBJECT
public:
    MainWindow();
private:

    QGridLayout mainLayout;
    QPushButton clearButton{"Clear"};
    KDTreeVisualisationWidget kdWidget;
};

#endif // MAINWINDOW_H
