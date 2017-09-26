#ifndef TRIMESH2D_H
#define TRIMESH2D_H

#include "common/point2d.h"
#include "triangle.h"

class TriMesh2D
{
    public:
        TriMesh2D();

        void addTri(Triangle toAdd);

        std::vector<Point2Dd> getVertices() const;

        std::vector<uint> getTris() const;

    private:
        std::vector<Point2Dd> vertices;
        std::vector<uint> tris;
};

#endif // TRIMESH2D_H
