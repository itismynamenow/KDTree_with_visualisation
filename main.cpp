#include <QApplication>
#include <QWidget>

#include <vector>

#include "mainwindow.h"
#include "kdtree.h"

using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
//    vector<vector<double>> points;
//    points.push_back(vector<double>{1,1});
//    points.push_back(vector<double>{1,2});
//    points.push_back(vector<double>{3,1});
//    points.push_back(vector<double>{4,1});
//    points.push_back(vector<double>{1,6});
//    points.push_back(vector<double>{4,4});
//    points.push_back(vector<double>{5,0});

//    KDTree tree(points);
}
