#ifndef DAGDELAUNAY_H
#define DAGDELAUNAY_H

#include "dagnode.h"
#include "utils/triutils.h"

class Triangulation;

class DagDelaunay
{
    public:
        DagDelaunay();

        DagNode* locate(Point2Dd &x, Triangulation *t);

        DagNode *getRoot();

private:

        DagNode *root;
        DagNode *locateRec(Point2Dd &x, Triangulation *t, DagNode *node);
};



#endif // DAGDELAUNAY_H
