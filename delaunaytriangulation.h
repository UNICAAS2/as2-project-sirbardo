#ifndef DELAUNAYTRIANGULATION_H
#define DELAUNAYTRIANGULATION_H

#include "common/point2d.h"
#include "triangle.h"
#include "dagdelaunay.h"

class DelaunayTriangulation
{
    public:
        DelaunayTriangulation();
        DelaunayTriangulation(Triangle a);
        void addPoint(Point2Dd newPoint);


        std::vector<Point2Dd> getVertices() const;
        std::vector<uint> getTris() const;
        std::vector<uint> getAdj() const;


    private:
        std::vector<Point2Dd> vertices;
        std::vector<uint> tris;
        std::vector<DagNode*> adj;
        DagDelaunay dag;
};

#endif // DELAUNAYTRIANGULATION_H
