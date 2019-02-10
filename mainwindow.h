#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <ctime>
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
    QPushButton randomButton{"Random"};
    QLineEdit lineEdit{"25"};
    KDTreeVisualisationWidget kdWidget;
    QHBoxLayout inputPanelLayout;
    QIntValidator inputValidator{1, 9999};
private slots:
    void setRandomData();
};

#endif // MAINWINDOW_H
