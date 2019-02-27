#ifndef KDTREEVISUALISATIONWIDGET_H
#define KDTREEVISUALISATIONWIDGET_H

#include <QtWidgets>

#include "kdtree.h"

#include <iostream>
#include <chrono>
#include <vector>
using namespace std;
using namespace std::chrono;

class KDTreeVisualisationWidget: public QWidget
{
    Q_OBJECT
public:
    KDTreeVisualisationWidget();
    void setPoints(vector<QPoint> points);
public slots:
    void reset();
protected:
    virtual void mousePressEvent(QMouseEvent *mouseEvent) override;
    virtual void paintEvent(QPaintEvent *event) override;
    void sendPointsToKDTree();
    void drawSubtree(KDNode *node, QPainter *painter,int dimension, int minX, int maxX, int minY, int maxY);

    vector<QPoint> points;
    QPoint referencePoint;
    QPoint closestPoint;
    bool referencePointSet = false;
    bool closestPointFound = false;
    KDTree kdTree;
};

#endif // KDTREEVISUALISATIONWIDGET_H
