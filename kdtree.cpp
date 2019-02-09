#include "kdtree.h"
#include <cmath>
#include <iostream>

KDTree::KDTree()
{
    rootNode = nullptr;
}

KDTree::KDTree(vector<DOUBLES> points)
{
    rootNode = buildSubtree(points,0);
}

KDTree::~KDTree()
{
    deleteNodeRecursivly(rootNode);
}

KDNode KDTree::getNearestNode(DOUBLES point)
{
    dropedVolumes.clear();
    KDNode closestNode;
    double closestDistance = MAX_DOUBLE();
    searchForClosestNode(rootNode, point, DOUBLES(point.size(), MAX_DOUBLE()), DOUBLES(point.size(), MIN_DOUBLE()), closestNode, closestDistance);
    return closestNode;
}

void KDTree::setNewPoints(vector<DOUBLES> points)
{
    deleteNodeRecursivly(rootNode);
    rootNode = buildSubtree(points,0);
}

void KDTree::clear()
{
    dropedVolumes.clear();
    deleteNodeRecursivly(rootNode);
    rootNode = nullptr;
}

KDNode *KDTree::buildSubtree(vector<DOUBLES> points, int currentDimension)
{
    if(points.size() == 0){
        return nullptr;
    }
    KDNode *currentNode = new KDNode();
    //Find current dimension and sort points according to it
    int maxDimension = points[0].size();
    currentDimension = currentDimension % maxDimension;
    if(points.size() == 1){
        currentNode->setPoints(points[0]);
        currentNode->setDimension(currentDimension);
        return currentNode;
    }

    sort(points.begin(),points.end(),[currentDimension](const DOUBLES &a,const DOUBLES &b){return b[currentDimension]>a[currentDimension];});

    //Find middle element and turn its position into subtree root
    int medianIndex = points.size()/2;
    currentNode->setPoints(points[medianIndex]);
    currentNode->setDimension(currentDimension);

    //Build subtrees for child nodes if possible
    if(medianIndex > 0){
        vector<DOUBLES> leftPoints(points.begin(),points.begin()+medianIndex);
        currentNode->left = buildSubtree(leftPoints, currentDimension+1);
    }
    if(medianIndex < points.size()-1){
        vector<DOUBLES> rightPoints(points.begin()+medianIndex+1,points.end());
        currentNode->right = buildSubtree(rightPoints, currentDimension+1);
    }

    return currentNode;
}

void KDTree::searchForClosestNode(KDNode *node, const DOUBLES &point, DOUBLES maxBound, DOUBLES minBound, KDNode &bestNode, double &bestDistance)
{
    if(!node) return;
    //Check if distance to bounding volume is smaller than bestDistance
    if(bestDistance < distanceToBoundingVolume(maxBound, minBound, point)){
        dropedVolumes.push_back(maxBound);
        dropedVolumes.push_back(minBound);
        return;
    }
    //Update best distance
    double currentDistance = euclidianDistance(node->point,point);
    if(currentDistance < bestDistance){
        bestNode = *node;
        bestDistance = currentDistance;
    }
    //Create bounind volume for child nodes
    DOUBLES leftMinBound = minBound, leftMaxBound = maxBound,
            rightMinBound = minBound, rightMaxBound = maxBound;
    leftMaxBound[node->dimension] = node->point[node->dimension];
    rightMinBound[node->dimension] = node->point[node->dimension];
    //Find child closest to point and process it first
    if(node->left && node->right){
        double leftDistance = euclidianDistance(node->left->point,point);
        double rightDistance = euclidianDistance(node->right->point,point);
        if(leftDistance < rightDistance){
            searchForClosestNode(node->left, point, leftMaxBound, leftMinBound, bestNode, bestDistance);
            searchForClosestNode(node->right, point, rightMaxBound, rightMinBound, bestNode, bestDistance);
        }else{
            searchForClosestNode(node->right, point, rightMaxBound, rightMinBound, bestNode, bestDistance);
            searchForClosestNode(node->left, point, leftMaxBound, leftMinBound, bestNode, bestDistance);
        }
    }else if(node->left){
        searchForClosestNode(node->left, point, leftMaxBound, leftMinBound, bestNode, bestDistance);
    }else if(node->right){
        searchForClosestNode(node->right, point, rightMaxBound, rightMinBound, bestNode, bestDistance);
    }

}

double KDTree::manhattanDistance(const DOUBLES &a, const DOUBLES &b){
    double distance = 0;
    for(int i=0;i<a.size();i++){
        distance += abs(a[i]-b[i]);
    }
    return distance;
}

double KDTree::euclidianDistance(const DOUBLES &a, const DOUBLES &b)
{
    double distance = 0;
    for(int i=0;i<a.size();i++){
        distance += pow(a[i]-b[i],2);
    }
    return sqrt(distance);
}

//Incorrect but fast way to find nearest neighobr
KDNode *KDTree::lookForClosestNode(KDNode *node, DOUBLES point)
{
    double distance = manhattanDistance(node->point,point);
    double leftDistance = -1;
    double rightDistance = -1;
    KDNode * closestNode = nullptr;
    if(node->left && node->right){
        leftDistance = manhattanDistance(node->left->point,point);
        rightDistance = manhattanDistance(node->right->point,point);
        if(leftDistance < rightDistance){
            closestNode = lookForClosestNode(node->left, point);
        }else{
            closestNode = lookForClosestNode(node->right, point);
        }
    }else if(node->left){
        closestNode = lookForClosestNode(node->left, point);
    }else if(node->right){
        closestNode = lookForClosestNode(node->right, point);
    }

    if(closestNode){
        double closestNodeDistance = manhattanDistance(closestNode->point,point);
        if(distance < closestNodeDistance) return node;
        else return closestNode;
    }else{
        return node;
    }

}

void KDTree::deleteNodeRecursivly(KDNode *node)
{
    if(!node) return;
    if(node->left) deleteNodeRecursivly(node->left);
    if(node->right) deleteNodeRecursivly(node->right);
    delete node;
}

double KDTree::distanceToBoundingVolume(const DOUBLES &maxBound, const DOUBLES &minBound, const DOUBLES &point)
{
    //check if point is inside volume
    DOUBLES closestPoint = point;
    for(int i=0;i<point.size();i++){
        if(point[i] <= maxBound[i] && point[i] >= minBound[i]){
            //do nothing cuz point is "inside" bound
            closestPoint[i] = point[i];
        }else if(point[i] > maxBound[i]){
            closestPoint[i] = maxBound[i];
        }else if(point[i] < minBound[i]){
            closestPoint[i] = minBound[i];
        }
    }
    double distance = euclidianDistance(closestPoint,point);
    return distance;
}









































