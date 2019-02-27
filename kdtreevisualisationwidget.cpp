#include "kdtreevisualisationwidget.h"

KDTreeVisualisationWidget::KDTreeVisualisationWidget()
{
    this->setMinimumSize(800,800);
}

void KDTreeVisualisationWidget::setPoints(vector<QPoint> points)
{
    this->reset();
    this->points = points;
    this->sendPointsToKDTree();
    this->update();
}

void KDTreeVisualisationWidget::reset()
{
    points.clear();
    kdTree.clear();
    this->update();
    closestPointFound = false;
    referencePointSet = false;
}

void KDTreeVisualisationWidget::mousePressEvent(QMouseEvent *mouseEvent)
{
    bool callUpdate = false;
    cout<<mouseEvent->pos().x()<<" "<<mouseEvent->pos().y()<<endl;
    if(mouseEvent->buttons() == Qt::RightButton){
        referencePointSet = true;
        referencePoint = mouseEvent->pos();
        callUpdate = true;
    }else if(mouseEvent->buttons() == Qt::LeftButton){
        points.push_back(mouseEvent->pos());
        this->sendPointsToKDTree();
        callUpdate = true;
    }
    if(kdTree.rootNode && referencePointSet){
        KDNode closestNode = kdTree.getNearestNode(vector<double>{referencePoint.x(),referencePoint.y()});
        closestPoint = QPoint(closestNode.point[0],closestNode.point[1]);
        closestPointFound = true;
        callUpdate = true;
    }
    if(callUpdate) this->update();
}

void KDTreeVisualisationWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    if(kdTree.dropedVolumes.size()>0){
        for(int i=0;i<kdTree.dropedVolumes.size();i+=2){
            int x = max(kdTree.dropedVolumes[i+1][0],0.0);
            int y = max(kdTree.dropedVolumes[i+1][1],0.0);
            int width = min(kdTree.dropedVolumes[i][0]-(double)x,(double)this->size().width());
            int height = min(kdTree.dropedVolumes[i][1]-(double)y,(double)this->size().height());
            painter.setBrush(QBrush(Qt::cyan));
            painter.drawRect(x,y,width,height);
        }

    }
    if(kdTree.rootNode){
        drawSubtree(kdTree.rootNode, &painter,0 , 0, this->size().width(), 0, this->size().height());
    }

//    QPainterPath path;
//    path.setFillRule(Qt::WindingFill);
//    painter.setBrush(QBrush(Qt::yellow));
//    for(QPoint point: points){
//        path.addEllipse(point,5,5);
//    }
//    painter.drawPath(path);
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    for(QPoint point: points){
        painter.setBrush(QBrush(Qt::yellow));
        painter.drawEllipse(point,5,5);
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    cout<<" All point drawn in "<<duration<<" macroseconds"<<endl;
    if(referencePointSet){
        painter.setBrush(QBrush(Qt::green));
        painter.drawEllipse(referencePoint,8,8);
    }
    if(closestPointFound){
        painter.setBrush(QBrush(Qt::red));
        painter.drawEllipse(closestPoint,8,8);
    }


}

void KDTreeVisualisationWidget::sendPointsToKDTree()
{
    vector<vector<double>> pointVector;
    for(QPoint point: points){
        pointVector.push_back(vector<double>{point.x(),point.y()});
    }

    kdTree.setNewPoints(pointVector);
}

void KDTreeVisualisationWidget::drawSubtree(KDNode *node, QPainter *painter, int dimension, int minX, int maxX, int minY, int maxY)
{
    if(!node) return;
    painter->setPen(QPen(Qt::black,max(1,6-dimension)));
    if(dimension % 2 == 0){
        painter->drawLine(node->point[0],minY,node->point[0],maxY);
        drawSubtree(node->left, painter, dimension+1, minX, node->point[0], minY, maxY);
        drawSubtree(node->right, painter, dimension+1, node->point[0], maxX, minY, maxY);

    }
    else{
        painter->drawLine(minX,node->point[1],maxX,node->point[1]);
        drawSubtree(node->right, painter, dimension+1, minX, maxX, node->point[1], maxY);
        drawSubtree(node->left, painter, dimension+1, minX, maxX, minY, node->point[1]);
    }
    painter->setPen(QPen(Qt::black,1));
}















