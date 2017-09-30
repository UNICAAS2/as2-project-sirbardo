#include "voronoi.h"


Voronoi::Voronoi(Triangulation *t)
{
    DelaunayUtils::createVoronoi(t, this);
}

void Voronoi::addPoint(Point2Dd pointToAdd)
{
    vertices.push_back(pointToAdd);

}

void Voronoi::addEdge(uint a, uint b)
{

    edges.push_back(a);
    edges.push_back(b);

}

void Voronoi::addDiagram(std::vector<Point2Dd> v, std::vector<uint> e)
{
    vertices.clear();
    edges.clear();

    vertices = v;
    edges = e;

}

void Voronoi::refreshDiagram(Triangulation *tri)
{
    clearDiagram();
    t = tri;
    DelaunayUtils::createVoronoi(t, this);

}

void Voronoi::clearDiagram()
{
    vertices.clear();
    edges.clear();
}
