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

        std::vector<Point2Dd> getVertices() const;
        std::vector<uint> getTris() const;
        std::vector<DagNode*> getAdj() const;


    protected:
        std::vector<Point2Dd> vertices;
        std::vector<uint> tris;
        std::vector<DagNode*> adj;
        std::vector<bool> activeList;
        DagDelaunay *dag;
        void legalizeEdge(int tri, int edge);


};

#endif // DELAUNAYTRIANGULATION_H
