#include "kdtreevisualisationwidget.h"

KDTreeVisualisationWidget::KDTreeVisualisationWidget()
{
    this->setMinimumSize(800,800);
    timer.start(30);
//    connect(&timer,SIGNAL(timeout()),this,SLOT(update()));
}

void KDTreeVisualisationWidget::reset()
{
    points.clear();
    kdTree.clear();
    this->update();
    closestPointFound = false;
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

        vector<vector<double>> pointVector;
        for(QPoint point: points){
            pointVector.push_back(vector<double>{point.x(),point.y()});
        }

        kdTree.setNewPoints(pointVector);

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
    for(QPoint point: points){
        painter.setBrush(QBrush(Qt::white));
        painter.drawEllipse(point,7,7);
    }
    if(referencePointSet){
        painter.setBrush(QBrush(Qt::green));
        painter.drawEllipse(referencePoint,8,8);
    }
    if(closestPointFound){
        painter.setBrush(QBrush(Qt::red));
        painter.drawEllipse(closestPoint,8,8);
    }


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















