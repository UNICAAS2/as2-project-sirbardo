#include "dagdelaunay.h"

//Dag

DagDelaunay::DagDelaunay()
{
    root = new DagNode(0)
}

DagNode *DagDelaunay::getRoot() const
{
    return root;
}
