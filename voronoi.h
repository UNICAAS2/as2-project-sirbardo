#ifndef VORONOI_H
#define VORONOI_H

#include <utils/delaunayutils.h>


/**
 * @brief The Voronoi class
 * Holds information about vertices and edges to draw a Voronoi diagram - nothing else.
 * Very basic class to be able to draw it and not much more.
 */

class Voronoi
{

    public:

    Voronoi(Triangulation *t);
    void addPoint(Point2Dd pointToAdd);
    void addEdge(uint a, uint b);
    void addDiagram(std::vector<Point2Dd> v, std::vector<uint> e);



    void clearDiagram();
    void refreshDiagram(Triangulation *tri);

    protected:

    std::vector<Point2Dd> vertices;
    std::vector<uint> edges;
    Triangulation *t;

};

#endif // VORONOI_H
