#include "trimesh2d.h"

TriMesh2D::TriMesh2D()
{

}

void TriMesh2D::addTri(Triangle toAdd)
{



    for(Point2Dd & vtx : vertices)
    {
        if (toAdd.getA() == vtx)

    }

}

std::vector<Point2Dd> TriMesh2D::getVertices() const
{
    return vertices;
}

std::vector<uint> TriMesh2D::getTris() const
{
    return tris;
}
