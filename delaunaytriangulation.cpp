#include "delaunaytriangulation.h"
#include "dagdelaunay.h"

/**
 * @brief Triangulation::Triangulation
 * the default constructor builds a fresh triangulation
 * with the bounding triangle as its only triangle.
 */
Triangulation::Triangulation()
{
    dag = new DagDelaunay();

    Point2Dd BT_P1(1e+10, 0);
    Point2Dd BT_P2(0, 1e+10);
    Point2Dd BT_P3(-1e+10, -1e+10);

    vertices.push_back(BT_P1);
    vertices.push_back(BT_P2);
    vertices.push_back(BT_P3);

    tris.push_back(0);
    tris.push_back(1);
    tris.push_back(2);
    activeList.push_back(true);

    adj.push_back(dag->getRoot());
    adj.push_back(dag->getRoot());
    adj.push_back(dag->getRoot());

}

/**
 * @brief Triangulation::Triangulation
 * @param a first triangle
 */
Triangulation::Triangulation(Triangle a)
{
    dag = new DagDelaunay();

    vertices.push_back(a.getA());
    vertices.push_back(a.getB());
    vertices.push_back(a.getC());

    tris.push_back(0);
    tris.push_back(1);
    tris.push_back(2);
    activeList.push_back(true);

    adj.push_back(dag->getRoot());
    adj.push_back(dag->getRoot());
    adj.push_back(dag->getRoot());


}

/**
 * @brief Triangulation::clearTriangulation
 *
 * restores a triangulation to its original state, with a single triangle inside.
 */
void Triangulation::clearTriangulation()
{
    vertices.resize(3); //resize also destroys elements, so it should destroy each Point2Dd correctly
    tris.resize(3);

    for (DagNode* node : adj) //the dag takes care of freeing these nodes, I am just losing the references here
        node = nullptr;

    adj.clear(); //this SHOULD free the space that the references took
    adj.push_back(dag->getRoot());
    adj.push_back(dag->getRoot());
    adj.push_back(dag->getRoot());
    activeList.resize(1);

    //Clearing Dag

    dag->clearDag();
}

/**
 * @brief Triangulation::findOppositeVertex
 * @param v1 endpoint of edge
 * @param v2 endpoint of edge
 * @param adjTri adjacent triangle along v1-v2
 * @param edgeTriIsAdjacentOn will store what edge of the adjacent triangle the asking triangle is adjacent on
 * @return opposite vertex (index)
 */
uint Triangulation::findOppositeVertex(uint v1, uint v2, uint adjTri, uint &edgeTriIsAdjacentOn)
{
    if (tris[adjTri] == v1 || tris[adjTri] == v2)
        if (tris[adjTri+1] == v1 || tris[adjTri+2] == v2)
        {
            edgeTriIsAdjacentOn = 0;
            return tris[adjTri+2];
        }
        else
        {
            edgeTriIsAdjacentOn = 2;
            return tris[adjTri+1];
        }
    else
    {
        edgeTriIsAdjacentOn = 1;
        return tris[adjTri];
    }
}

DagDelaunay *Triangulation::getDag()
{
    return dag;
}

/**
 * @brief Triangulation::findOppositeVertex
 * @param v1 endpoint of edge
 * @param v2 endpoint of edge
 * @param adjTri adjacent triangle along v1-v2
 * @return opposite vertex (index)
 */
uint Triangulation::findOppositeVertex(uint v1, uint v2, uint adjTri)
{
    if (tris[adjTri] == v1 || tris[adjTri] == v2)
        if (tris[adjTri+1] == v1 || tris[adjTri+2] == v2)
            return tris[adjTri+2];
        else
        {

            return tris[adjTri+1];
        }
    else
        return tris[adjTri];
}

/**
 * @brief Triangulation::addPoint adds a point to the triangulation, triangulates, and flips edges if needed.
 * @param newPoint
 *
 */
void Triangulation::addPoint(Point2Dd newPoint)
{
    DagNode* oldTri = dag->locate(newPoint, this); //find the tri that contains the points
    uint oldTriIndex = oldTri->getTIndex();

    Point2Dd a = vertices[tris[oldTriIndex]];
    Point2Dd b = vertices[tris[oldTriIndex+1]];
    Point2Dd c = vertices[tris[oldTriIndex+2]];

    if (newPoint == a || newPoint == b || newPoint == c) return;    //if the point is equal to one of the vertices of the
                                                                    //triangle it lies in, then there's nothing new to do.

    vertices.push_back(newPoint); //if not, add the point to the triangulation.
    uint newPointIndex = vertices.size()-1;
    DagNode* newTri1Node;
    DagNode* newTri2Node;
    DagNode* newTri3Node;
    uint newTri1, newTri2, newTri3;


    //triangle being split is not active anymore
    activeList[oldTriIndex/3] = false;

    //triangle 1
    tris.push_back(tris[oldTriIndex]); //new vertices in counter-clockwise order
    tris.push_back(tris[oldTriIndex+1]);
    tris.push_back(newPointIndex);
    newTri1 = tris.size()-3;
    newTri1Node = new DagNode(newTri1);
    dag->addNode(newTri1Node);
    oldTri->addChild(newTri1Node);
    activeList.push_back(true);

    //triangle 2
    tris.push_back(newPointIndex);
    tris.push_back(tris[oldTriIndex+1]);
    tris.push_back(tris[oldTriIndex+2]);
    newTri2 = tris.size()-3;
    newTri2Node = new DagNode(newTri2);
    dag->addNode(newTri2Node);
    oldTri->addChild(newTri2Node);
    activeList.push_back(true);


    //triangle 3
    tris.push_back(tris[oldTriIndex]);
    tris.push_back(newPointIndex);
    tris.push_back(tris[oldTriIndex+2]);
    newTri3 = tris.size()-3;
    newTri3Node = new DagNode(newTri3);
    dag->addNode(newTri3Node);
    oldTri->addChild(newTri3Node);
    activeList.push_back(true);

    //adjacencies of the first triangle

    adj.push_back(adj[oldTriIndex]); //along edge 01->adjacent to its parent's adjacent along its 01 edge, 12->triangle 2, 20->triangle 3
    adj.push_back(newTri2Node);
    adj.push_back(newTri3Node);

    uint adjTri = adj[oldTriIndex]->getTIndex();
    uint v1, v2;

    v1 = tris[newTri1];
    v2 = tris[newTri1+1];

    if (adjTri != 0) //I decided that only the tris incident to the bounding tri can be adjacent to it, so 0 means it's the BT itself
    {
        //I need to know what edge of the adjacent triangle this new triangle is adjacent on.
        //
        if (tris[adjTri] != v1 && tris[adjTri] != v2) adj[adjTri+1] = newTri1Node; //if the 0-vertex is not part of the shared edge, then 12 is the edge I am adjacent on.
        else if (tris[adjTri+1] != v1 && tris[adjTri+1] != v2) adj[adjTri+2] = newTri1Node; //etc.
        else adj[adjTri] = newTri1Node;
    }

    //adjacencies of the second triangle

    adj.push_back(newTri1Node); //along edge 01->triangle 1, 12->parentAdj along parent's 12, 20->triangle 3
    adj.push_back(adj[oldTriIndex+1]);
    adj.push_back(newTri3Node);

    adjTri = adj[oldTriIndex+1]->getTIndex();

    v1 = tris[newTri2+1];
    v2 = tris[newTri2+2];

    if (adjTri != 0)
    {
        if (tris[adjTri] != v1 && tris[adjTri] != v2) adj[adjTri+1] = newTri2Node;
        else if (tris[adjTri+1] != v1 && tris[adjTri+1] != v2) adj[adjTri+2] = newTri2Node;
        else adj[adjTri] = newTri2Node;
    }

    //adjacencies of the third triangle

    adj.push_back(newTri1Node); //along edge 01->triangle 1, 12->triangle 2, 20->parentAdj along parent's 20
    adj.push_back(newTri2Node);
    adj.push_back(adj[oldTriIndex+2]);

    adjTri = adj[oldTriIndex+2]->getTIndex();

    v1 = tris[newTri3];
    v2 = tris[newTri3+2];

    if (adjTri != 0)
    {
        if (tris[adjTri] != v1 && tris[adjTri] != v2) adj[adjTri+1] = newTri3Node;
        else if (tris[adjTri+1] != v1 && tris[adjTri+1] != v2) adj[adjTri+2] = newTri3Node;
        else adj[adjTri] = newTri3Node;
    }





    legalizeEdge(newTri1, 0);//CCW order, I need to legalize the 01 edge of the newTri1, 12 of newTri2, and 20 of newTri3.
    legalizeEdge(newTri2, 1);
    legalizeEdge(newTri3, 2);



}

std::vector<Point2Dd> &Triangulation::getVertices()
{
    return vertices;
}


/**
 * @brief Triangulation::legalizeEdge
 * @param tri index of tri whose edge could be flipped
 * @param edge edge that could be flipped (seen as offset 0, 1 or 2)
 * NOTE: this function could be better written by using the edge itself as an offset, I did actually implement
 * a stub and it works, but it would have taken too long to replace this. It's not pretty, but it works fast.
 */
void Triangulation::legalizeEdge(uint tri, uint edge)
{

    DagNode* adjTriNode = adj[tri+edge];
    uint adjTri = adjTriNode->getTIndex();
    uint oppositeVertex;
    uint vertex1, vertex2, vertexNotEdge, adjAdj;


    switch(edge)
    {

    case 0:

        vertex1 = tris[tri];
        vertex2 = tris[tri+1];
        vertexNotEdge = tris[tri+2];


        //I need to find the opposite vertex and what edge of its opposite triangle the new triangle lies on.
        if (tris[adjTri] != vertex1 && tris[adjTri] != vertex2)
        {
            oppositeVertex = tris[adjTri];
            adjAdj = 1;
        }
        else if (tris[adjTri+1] != vertex1 && tris[adjTri+1] != vertex2)
        {
            oppositeVertex = tris[adjTri+1];
            adjAdj = 2;
        }
        else
        {
            oppositeVertex = tris[adjTri+2];
            adjAdj = 0;
        }



        //The first check deals with the "point-on-edge" case.
        if (geomUtils::isPointOnLine(vertices[vertexNotEdge], vertices[vertex1], vertices[vertex2]) ||
            DelaunayTriangulation::Checker::isPointLyingInCircle(vertices[vertex1], vertices[vertex2], vertices[vertexNotEdge], vertices[oppositeVertex], false))
        {
            /*FLIP*/


            //deactivate the two triangles involved
            activeList[tri/3] = false;
            activeList[adj[tri+edge]->getTIndex()/3] = false;

            //New Triangle 1
            tris.push_back(vertex1);
            tris.push_back(oppositeVertex);
            tris.push_back(vertexNotEdge);
            DagNode* tri1Node = new DagNode (tris.size()-3);
            dag->addNode(tri1Node);
            activeList.push_back(true);

            //New Triangle 2
            tris.push_back(oppositeVertex);
            tris.push_back(vertex2);
            tris.push_back(vertexNotEdge);
            DagNode* tri2Node = new DagNode(tris.size()-3);
            dag->addNode(tri2Node);
            activeList.push_back(true);

            //Refresh Dag
            adjTriNode->addChild(tri1Node);
            adjTriNode->addChild(tri2Node);
            adj[adjTri+adjAdj]->addChild(tri1Node);
            adj[adjTri+adjAdj]->addChild(tri2Node);


            //Adjacencies of the first triangle
            adj.push_back(adj[adjTri+((adjAdj+1)%3)]);
            adj.push_back(tri2Node);
            adj.push_back(adj[tri+2]);

            //Adjacencies of the second triangle
            adj.push_back(adj[adjTri+((adjAdj+2)%3)]);
            adj.push_back(adj[tri+1]);
            adj.push_back(tri1Node);

            //Refresh Adjacencies of incident triangles

            //Tris incident to tri1
            uint incidentTri1 = adj[adjTri+((adjAdj+1)%3)]->getTIndex(); //tri adjacent along first edge
            uint startingVertex1; //will store along what edge of the incident tri tri1 lies on

            if (tris[incidentTri1] !=  vertex1 && tris[incidentTri1] != oppositeVertex) startingVertex1 = 1;
            else if (tris[incidentTri1+1] !=  vertex1 && tris[incidentTri1+1] != oppositeVertex) startingVertex1 = 2;
            else startingVertex1 = 0;

            adj[incidentTri1+startingVertex1] = tri1Node;

            uint startingVertex2, incidentTri2;
            incidentTri2 = adj[tri+2]->getTIndex();

            if (tris[incidentTri2] !=  vertexNotEdge && tris[incidentTri2] != vertex1) startingVertex2 = 1;
            else if (tris[incidentTri2+1] !=  vertexNotEdge && tris[incidentTri2+1] != vertex1) startingVertex2 = 2;
            else startingVertex2 = 0;

            adj[incidentTri2+startingVertex2] = tri1Node;


            //Tris incident to tri2

            uint startingVertex3, incidentTri3;
            incidentTri3 = adj[adjTri+((adjAdj+2)%3)]->getTIndex();
            if (tris[incidentTri3] !=  vertex2 && tris[incidentTri3] != oppositeVertex) startingVertex3 = 1;
            else if (tris[incidentTri3+1] !=  vertex2 && tris[incidentTri3+1] != oppositeVertex) startingVertex3 = 2;
            else startingVertex3 = 0;

            adj[incidentTri3+startingVertex3] = tri2Node;


            uint startingVertex4, incidentTri4;
            incidentTri4 = adj[tri+1]->getTIndex();
            if (tris[incidentTri4] !=  vertexNotEdge && tris[incidentTri4] != vertex2) startingVertex4 = 1;
            else if (tris[incidentTri4+1] !=  vertexNotEdge && tris[incidentTri4+1] != vertex2) startingVertex4 = 2;
            else startingVertex4 = 0;

            adj[incidentTri4+startingVertex4] = tri2Node;


            uint newTri1 = tris.size()-6;
            uint newTri2 = tris.size()-3;

            if (adj[newTri1]->getTIndex() != 0) legalizeEdge(newTri1, 0);
            if (adj[newTri2]->getTIndex() != 0) legalizeEdge(newTri2, 0);

            //legalizeEdge(tris.size()-6, 0);
            //legalizeEdge(tris.size()-3, 0);

        }

        break;

    case 1:

        vertex1 = tris[tri+1];
        vertex2 = tris[tri+2];
        vertexNotEdge = tris[tri];

        if (tris[adjTri] != vertex1 && tris[adjTri] != vertex2)
        {
            oppositeVertex = tris[adjTri];
            adjAdj = 1;
        }
        else if (tris[adjTri+1] != vertex1 && tris[adjTri+1] != vertex2)
        {
            oppositeVertex = tris[adjTri+1];
            adjAdj = 2;
        }
        else
        {
            oppositeVertex = tris[adjTri+2];
            adjAdj = 0;
        }

        if (geomUtils::isPointOnLine(vertices[vertexNotEdge], vertices[vertex1], vertices[vertex2]) ||
            DelaunayTriangulation::Checker::isPointLyingInCircle(vertices[vertex1], vertices[vertex2], vertices[vertexNotEdge], vertices[oppositeVertex], false))
        {
            /*FLIP*/


            //deactivate the two triangles involved
            activeList[tri/3] = false;
            activeList[adj[tri+edge]->getTIndex()/3] = false;

            //New Triangle 1
            tris.push_back(vertex1);
            tris.push_back(oppositeVertex);
            tris.push_back(vertexNotEdge);
            DagNode* tri1Node = new DagNode (tris.size()-3);
            dag->addNode(tri1Node);


            activeList.push_back(true);

            //New Triangle 2
            tris.push_back(oppositeVertex);
            tris.push_back(vertex2);
            tris.push_back(vertexNotEdge);
            DagNode* tri2Node = new DagNode(tris.size()-3);
            dag->addNode(tri2Node);

            activeList.push_back(true);

            //Refresh Dag
            adjTriNode->addChild(tri1Node);
            adjTriNode->addChild(tri2Node);
            adj[adjTri+adjAdj]->addChild(tri1Node);
            adj[adjTri+adjAdj]->addChild(tri2Node);


            //Adjacencies of the first triangle
            adj.push_back(adj[adjTri+((adjAdj+1)%3)]);
            adj.push_back(tri2Node);
            adj.push_back(adj[tri]);

            //Adjacencies of the second triangle
            adj.push_back(adj[adjTri+((adjAdj+2)%3)]);
            adj.push_back(adj[tri+2]);
            adj.push_back(tri1Node);

            //Refresh Adjacencies of incident triangles

            //Tris incident to tri1
            uint incidentTri1 = adj[adjTri+((adjAdj+1)%3)]->getTIndex(); //tri adjacent along first edge
            uint startingVertex1; //will store along what edge of the incident tri tri1 lies on

            if (tris[incidentTri1] !=  vertex1 && tris[incidentTri1] != oppositeVertex) startingVertex1 = 1;
            else if (tris[incidentTri1+1] !=  vertex1 && tris[incidentTri1+1] != oppositeVertex) startingVertex1 = 2;
            else startingVertex1 = 0;

            adj[incidentTri1+startingVertex1] = tri1Node;

            uint startingVertex2, incidentTri2;
            incidentTri2 = adj[tri]->getTIndex();
            if (tris[incidentTri2] !=  vertexNotEdge && tris[incidentTri2] != vertex1) startingVertex2 = 1;
            else if (tris[incidentTri2+1] !=  vertexNotEdge && tris[incidentTri2+1] != vertex1) startingVertex2 = 2;
            else startingVertex2 = 0;

            adj[incidentTri2+startingVertex2] = tri1Node;


            //Tris incident to tri2

            uint startingVertex3, incidentTri3;
            incidentTri3 = adj[adjTri+((adjAdj+2)%3)]->getTIndex();
            if (tris[incidentTri3] !=  vertex2 && tris[incidentTri3] != oppositeVertex) startingVertex3 = 1;
            else if (tris[incidentTri3+1] !=  vertex2 && tris[incidentTri3+1] != oppositeVertex) startingVertex3 = 2;
            else startingVertex3 = 0;

            adj[incidentTri3+startingVertex3] = tri2Node;


            uint startingVertex4, incidentTri4;
            incidentTri4 = adj[tri+2]->getTIndex();
            if (tris[incidentTri4] !=  vertexNotEdge && tris[incidentTri4] != vertex2) startingVertex4 = 1;
            else if (tris[incidentTri4+1] !=  vertexNotEdge && tris[incidentTri4+1] != vertex2) startingVertex4 = 2;
            else startingVertex4 = 0;

            adj[incidentTri4+startingVertex4] = tri2Node;


            uint newTri1 = tris.size()-6;
            uint newTri2 = tris.size()-3;

            if (adj[newTri1]->getTIndex() != 0) legalizeEdge(newTri1, 0);
            if (adj[newTri2]->getTIndex() != 0) legalizeEdge(newTri2, 0);

            //legalizeEdge(tris.size()-6, 0);
            //legalizeEdge(tris.size()-3, 0);

        }
        break;

    case 2:
        vertex1 = tris[tri+2];
        vertex2 = tris[tri];
        vertexNotEdge = tris[tri+1];

        if (tris[adjTri] != vertex1 && tris[adjTri] != vertex2)
        {
            oppositeVertex = tris[adjTri];
            adjAdj = 1;
        }
        else if (tris[adjTri+1] != vertex1 && tris[adjTri+1] != vertex2)
        {
            oppositeVertex = tris[adjTri+1];
            adjAdj = 2;
        }
        else
        {
            oppositeVertex = tris[adjTri+2];
            adjAdj = 0;
        }

        if (geomUtils::isPointOnLine(vertices[vertexNotEdge], vertices[vertex1], vertices[vertex2]) ||
            DelaunayTriangulation::Checker::isPointLyingInCircle(vertices[vertex1], vertices[vertex2], vertices[vertexNotEdge], vertices[oppositeVertex], false))
        {
            /*FLIP*/


            //deactivate the two triangles involved
            activeList[tri/3] = false;
            activeList[adj[tri+edge]->getTIndex()/3] = false;


            //New Triangle 1
            tris.push_back(vertex1);
            tris.push_back(oppositeVertex);
            tris.push_back(vertexNotEdge);
            DagNode* tri1Node = new DagNode (tris.size()-3);
            dag->addNode(tri1Node);

            activeList.push_back(true);

            //New Triangle 2
            tris.push_back(oppositeVertex);
            tris.push_back(vertex2);
            tris.push_back(vertexNotEdge);
            DagNode* tri2Node = new DagNode(tris.size()-3);
            dag->addNode(tri2Node);
            activeList.push_back(true);

            //Refresh Dag
            adjTriNode->addChild(tri1Node);
            adjTriNode->addChild(tri2Node);
            adj[adjTri+adjAdj]->addChild(tri1Node);
            adj[adjTri+adjAdj]->addChild(tri2Node);


            //Adjacencies of the first triangle
            adj.push_back(adj[adjTri+((adjAdj+1)%3)]);
            adj.push_back(tri2Node);
            adj.push_back(adj[tri+1]);

            //Adjacencies of the second triangle
            adj.push_back(adj[adjTri+((adjAdj+2)%3)]);
            adj.push_back(adj[tri]);
            adj.push_back(tri1Node);

            //Refresh Adjacencies of incident triangles

            //Tris incident to tri1
            uint incidentTri1 = adj[adjTri+((adjAdj+1)%3)]->getTIndex(); //tri adjacent along first edge
            uint startingVertex1; //will store along what edge of the incident tri tri1 lies on

            if (tris[incidentTri1] !=  vertex1 && tris[incidentTri1] != oppositeVertex) startingVertex1 = 1;
            else if (tris[incidentTri1+1] !=  vertex1 && tris[incidentTri1+1] != oppositeVertex) startingVertex1 = 2;
            else startingVertex1 = 0;

            adj[incidentTri1+startingVertex1] = tri1Node;

            uint startingVertex2, incidentTri2;
            incidentTri2 = adj[tri+1]->getTIndex();
            if (tris[incidentTri2] !=  vertexNotEdge && tris[incidentTri2] != vertex1) startingVertex2 = 1;
            else if (tris[incidentTri2+1] !=  vertexNotEdge && tris[incidentTri2+1] != vertex1) startingVertex2 = 2;
            else startingVertex2 = 0;

            adj[incidentTri2+startingVertex2] = tri1Node;


            //Tris incident to tri2

            uint startingVertex3, incidentTri3;
            incidentTri3 = adj[adjTri+((adjAdj+2)%3)]->getTIndex();
            if (tris[incidentTri3] !=  vertex2 && tris[incidentTri3] != oppositeVertex) startingVertex3 = 1;
            else if (tris[incidentTri3+1] !=  vertex2 && tris[incidentTri3+1] != oppositeVertex) startingVertex3 = 2;
            else startingVertex3 = 0;

            adj[incidentTri3+startingVertex3] = tri2Node;


            uint startingVertex4, incidentTri4;
            incidentTri4 = adj[tri]->getTIndex();
            if (tris[incidentTri4] !=  vertexNotEdge && tris[incidentTri4] != vertex2) startingVertex4 = 1;
            else if (tris[incidentTri4+1] !=  vertexNotEdge && tris[incidentTri4+1] != vertex2) startingVertex4 = 2;
            else startingVertex4 = 0;

            adj[incidentTri4+startingVertex4] = tri2Node;


            uint newTri1 = tris.size()-6;
            uint newTri2 = tris.size()-3;

            if (adj[newTri1]->getTIndex() != 0) legalizeEdge(newTri1, 0);
            if (adj[newTri2]->getTIndex() != 0) legalizeEdge(newTri2, 0);

            //legalizeEdge(tris.size()-6, 0);
            //legalizeEdge(tris.size()-3, 0);

        }
        break;



    }










}
std::vector<uint> &Triangulation::getTris()
{
    return tris;
}

std::vector<DagNode *> &Triangulation::getAdj()
{
    return adj;
}

std::vector<bool> &Triangulation::getActiveList()
{
    return activeList;
}
