#ifndef DAGDELAUNAY_H
#define DAGDELAUNAY_H

#include "dagnode.h"
#include "utils/geomutils.h"

class Triangulation;


/**
 * @brief The DagDelaunay class
 * This is the dag that is doubly linked with the Triangulation. It can locate a point inside a triangle, and as such
 * acts as the search structure for the Triangulation.
 */
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
        std::vector<DagNode*> allNodes; //Pointers to every node in the dag, used to free the memory when killed.
        DagNode* locateRec(Point2Dd &x, Triangulation *t, DagNode *node);
};



#endif // DAGDELAUNAY_H
