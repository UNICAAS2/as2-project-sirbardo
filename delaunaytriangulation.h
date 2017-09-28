#ifndef DELAUNAYTRIANGULATION_H
#define DELAUNAYTRIANGULATION_H

#include "triangle.h"
#include "utils/delaunay_checker.h"

class DagNode;
class DagDelaunay;

class Triangulation
{
    public:
        Triangulation();
        Triangulation(Triangle a);

        void addPoint(Point2Dd newPoint);

        std::vector<Point2Dd> &getVertices();
        std::vector<uint> &getTris();
        std::vector<DagNode*> &getAdj();
        std::vector<bool> &getActiveList();

protected:
        std::vector<Point2Dd> vertices;
        std::vector<uint> tris;
        std::vector<DagNode*> adj;
        std::vector<bool> activeList;
        DagDelaunay *dag;
        void legalizeEdge(int tri, int edge);


};

#endif // DELAUNAYTRIANGULATION_H
