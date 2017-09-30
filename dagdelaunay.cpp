#include "dagdelaunay.h"
#include "delaunaytriangulation.h"

//Dag

DagDelaunay::DagDelaunay()
{
    root = new DagNode(0); // 0 is the index of the Bounding Triangle.
    allNodes.push_back(root); // list of existing nodes is refreshed.
}

DagNode *DagDelaunay::getRoot()
{
    return root;
}

void DagDelaunay:: clearDag()
{
    for (int i = 1; i<allNodes.size(); i++)
    {
        delete allNodes[i]; //destroy every node of the DAG
        allNodes[i] = nullptr;
    }

    allNodes.resize(1); //should delete all references to previously existing nodes
    root->getChildren().resize(0); //should delete previously existing references to children
}

DagNode *DagDelaunay::locate(Point2Dd &x, Triangulation *t)
{
    return locateRec(x, t, root);
}

void DagDelaunay::addNode(DagNode *toAdd)
{
    allNodes.push_back(toAdd);
}


/**
 * @brief recursive lookup in the DAG
 * @param x point to locate
 * @param t triangulation
 * @param node node that is being checked at this recursive step
 * @return node that the point is located in
 *
 * NOTE: Something cool here, that I exploited to easily include the "Point-on-edge" case, is the fact that
 * if the point lies on an edge or on a vertex of a given triangle, the lookup returns the first child that
 * the point is found in, so there are no unexpected behaviors here, because I know a priori that it will
 * always return a given child. It could only go "wrong" if queried with a point that lies outside the bounding triangle,
 * but it's impossible given the viewer at hand.
 */
DagNode *DagDelaunay::locateRec(Point2Dd &x, Triangulation *t, DagNode* node)
{

    if (node->isLeaf())
        return node;


//#pragma omp parallel for
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
