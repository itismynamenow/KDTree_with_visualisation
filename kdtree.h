#ifndef KDTREE_H
#define KDTREE_H
#include <vector>
#include <algorithm>
#include <limits>

#define DOUBLES vector<double>
#define MAX_DOUBLE std::numeric_limits<double>::max
#define MIN_DOUBLE std::numeric_limits<double>::min

using namespace std;

struct KDNode{
    KDNode *left, *right;
    DOUBLES point;
    int dimension;
    KDNode(){
        left = nullptr;
        right = nullptr;
    }
    KDNode(DOUBLES point): KDNode(){
        this->point = point;
    }
    void setPoints(DOUBLES point){
        this->point = point;
        left = nullptr;
        right = nullptr;
    }
    void setDimension(int dimension){
        this->dimension = dimension;
    }
};

class KDTree
{
public:
    KDTree();
    KDTree(vector<DOUBLES> points);
    ~KDTree();
    KDNode getNearestNode(DOUBLES point);
    void setNewPoints(vector<DOUBLES> points);
    void clear();

    KDNode *rootNode;
    vector<DOUBLES> dropedVolumes;
protected:

    KDNode* buildSubtree(vector<DOUBLES> points, int currentDimension);
    void searchForClosestNode(KDNode *node,const DOUBLES &point, DOUBLES maxBound, DOUBLES minBound, KDNode &bestNode, double &bestDistance);
    KDNode *lookForClosestNode(KDNode *node, DOUBLES point);
    double manhattanDistance(const DOUBLES &a, const DOUBLES &b);
    double euclidianDistance(const DOUBLES &a, const DOUBLES &b);
    void deleteNodeRecursivly(KDNode *node);
    double distanceToBoundingVolume(const DOUBLES &maxBound, const DOUBLES &minBound, const DOUBLES &point);
};

#endif // KDTREE_H
