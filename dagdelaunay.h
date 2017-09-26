#ifndef DAGDELAUNAY_H
#define DAGDELAUNAY_H

#include "dagnode.h"
#include "

class DagDelaunay
{
    public:
        DagDelaunay();
        DagNode* locate(Point2Dd);


        DagNode *getRoot() const;

private:

        DagNode *root;
};



#endif // DAGDELAUNAY_H
