#ifndef DELAUNAYTRIANGULATION_H
#define DELAUNAYTRIANGULATION_H

#include "triangle.h"
#include "utils/delaunay_checker.h"
#include "utils/delaunayutils.h"

class DagNode;
class DagDelaunay;

/**
 * @brief The Triangulation class
 *
 * This is where all the magic happens. This data structure holds data about its triangles and its vertices,
 * and since (even though it's called Triangulation) it is a Delaunay Triangulation, it can legalize its
 * own edges whenever a point is added.
 *
 * The peculiarity about this data structure is that, for the sake of efficiency, it holds a lot of information about adjacencies between
 * its triangles.
 *
 */


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
        DagDelaunay *getDag();


        void clearTriangulation();

protected:
        std::vector<Point2Dd> vertices; //Holds data about vertices.

        std::vector<uint> tris; //Holds data about triangles (even the "dead" ones that have been split).
                                //Triangles are at i*3, i=0...n, i+0, i+1 and i+2 are the indices of its vertices in ccw order

        std::vector<DagNode*> adj;  //i*3, i=0...n, it grows together with tris, but instead of holding indices about vertices
                                    //it holds a pointer to the dagNode of the adjacent tri.
                                    //i+0 = adjacent along the first edge, i+1 = second edge, i+2 = third edge.

        std::vector<bool> activeList;   //boolean list of triangles, for every triangle to ever exist in the triangulation
                                        //it holds whether or not it's active at a given point in time.
        DagDelaunay *dag;

        void legalizeEdge(uint tri, uint edge);

        
        uint findOppositeVertex(uint v1, uint v2, uint adjTri);
        uint findOppositeVertex(uint v1, uint v2, uint adjTri, uint &edgeTriIsAdjacentOn);

};

#endif // DELAUNAYTRIANGULATION_H
