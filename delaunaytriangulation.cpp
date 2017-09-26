#include "delaunaytriangulation.h"

DelaunayTriangulation::DelaunayTriangulation()
{
    Point2Dd BT_P1(1e+10, 0);
    Point2Dd BT_P2(0, 1e+10);
    Point2Dd BT_P3(-1e+10, -1e+10);

    vertices.push_back(BT_P1);
    vertices.push_back(BT_P2);
    vertices.push_back(BT_P3);

    tris.push_back(0);
    tris.push_back(1);
    tris.push_back(2);

    adj.push_back(dag.getRoot());
    adj.push_back(dag.getRoot());
    adj.push_back(dag.getRoot());
}

DelaunayTriangulation::DelaunayTriangulation(Triangle a)
{
    vertices.push_back(a.getA());
    vertices.push_back(a.getB());
    vertices.push_back(a.getC());

    tris.push_back(0);
    tris.push_back(1);
    tris.push_back(2);

    adj.push_back(dag.getRoot());
    adj.push_back(dag.getRoot());
    adj.push_back(dag.getRoot());

}

void DelaunayTriangulation::addPoint(Point2Dd newPoint)
{
    vertices.push_back(newPoint);

    DagNode* oldTri = dag.locate(newPoint);

    uint i = oldTri->getTIndex();

    //triangle 1
    tris.push_back(tris[i]);
    tris.push_back(tris[i+1]);
    tris.push_back(newPoint);

    //triangle 2
    tris.push_back(newPoint);
    tris.push_back(tris[i+1]);
    tris.push_back(tris[i+2]);

    //triangle 3
    tris.push_back(tris[i]);
    tris.push_back(newPoint);
    tris.push_back(tris[i+2]);

    current = tris.size();

    adj.push_back(adj[i]); //along edge 01->parentAdj, 12->triangle 2, 20->triangle 3
    adj.push_back(adj);
    adj.push_back(adj[i]);
}

std::vector<Point2Dd> DelaunayTriangulation::getVertices() const
{
    return vertices;
}

std::vector<uint> DelaunayTriangulation::getTris() const
{
    return tris;
}

std::vector<uint> DelaunayTriangulation::getAdj() const
{
    return adj;
}
