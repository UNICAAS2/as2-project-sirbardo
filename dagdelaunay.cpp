#include "dagdelaunay.h"
#include "delaunaytriangulation.h"

//Dag

DagDelaunay::DagDelaunay()
{
    root = new DagNode(0);
    allNodes.push_back(root);
}

DagNode *DagDelaunay::getRoot()
{
    return root;
}

void DagDelaunay:: clearDag()
{
    for (int i = 1; i<allNodes.size(); i++)
    {
        delete allNodes[i];
        allNodes[i] = nullptr;
    }

    allNodes.resize(1);
    root->getChildren().resize(0);
}

DagNode *DagDelaunay::locate(Point2Dd &x, Triangulation *t)
{
    return locateRec(x, t, root);
}

void DagDelaunay::addNode(DagNode *toAdd)
{
    allNodes.push_back(toAdd);
}

DagNode *DagDelaunay::locateRec(Point2Dd &x, Triangulation *t, DagNode* node)
{

    if (node->isLeaf())
        return node;


#pragma omp parallel for
    for(DagNode* child : node->getChildren())
    {
        Point2Dd a = t->getVertices()[t->getTris()[child->getTIndex()]];
        Point2Dd b = t->getVertices()[t->getTris()[child->getTIndex()+1]];
        Point2Dd c = t->getVertices()[t->getTris()[child->getTIndex()+2]];
        if (geomUtils::triContainsPoint(a, b, c, x))
        {
            return locateRec(x, t, child);
        }
    }

}
