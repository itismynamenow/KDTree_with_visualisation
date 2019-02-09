#include "mainwindow.h"

MainWindow::MainWindow()
{
    mainLayout.addWidget(&kdWidget,0,0);
    mainLayout.addWidget(&clearButton,1,0);
    setLayout(&mainLayout);
    connect(&clearButton, SIGNAL(clicked()), &kdWidget, SLOT(reset()));
}

