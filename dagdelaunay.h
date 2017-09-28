#ifndef DAGDELAUNAY_H
#define DAGDELAUNAY_H

#include "dagnode.h"
#include "utils/geomutils.h"

class Triangulation;

class DagDelaunay
{
    public:
        DagDelaunay();

        DagNode* locate(Point2Dd &x, Triangulation *t);
        void addNode(DagNode*toAdd);

        DagNode* getRoot();

        void clearDag();
private:

        DagNode* root;
        std::vector<DagNode*> allNodes;
        DagNode* locateRec(Point2Dd &x, Triangulation *t, DagNode *node);
};



#endif // DAGDELAUNAY_H
