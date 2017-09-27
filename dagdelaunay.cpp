#include "dagdelaunay.h"
#include "delaunaytriangulation.h"

//Dag

DagDelaunay::DagDelaunay()
{
    root = new DagNode(0);
}

DagNode* DagDelaunay::getRoot()
{
    return root;
}

DagNode* DagDelaunay::locate(Point2Dd x, Triangulation *t)
{

    return locateRec(x, t, root);

}

DagNode* DagDelaunay::locateRec(Point2Dd x, Triangulation *t, DagNode* node)
{

    qDebug()<<"a";
    if (node->isLeaf())
        return node;
    qDebug()<<"b";

    for(DagNode* child : node->getChildren())
    {
        if (TriUtils::triContainsPoint(Triangle(  t->getVertices()[t->getTris()[child->getTIndex()]],
        t->getVertices()[t->getTris()[child->getTIndex()+1]],
        t->getVertices()[t->getTris()[child->getTIndex()+2]]),
        x))
        {
            qDebug()<<"c";
            return locateRec(x, t, child);
        }
    }

    qDebug()<<"Oh no!";
}
