#include "delaunaytriangulation.h"
#include "dagdelaunay.h"

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

void Triangulation::addPoint(Point2Dd newPoint)
{
    vertices.push_back(newPoint);
    int newPointIndex = vertices.size()-1;
    DagNode* oldTri = dag->locate(newPoint, this);
    DagNode* newTri1Node;
    DagNode* newTri2Node;
    DagNode* newTri3Node;
    int newTri1, newTri2, newTri3;

    uint i = oldTri->getTIndex();

    //triangle being split is not active anymore
    activeList[i/3] = false;

    //triangle 1
    tris.push_back(tris[i]);
    tris.push_back(tris[i+1]);
    tris.push_back(newPointIndex);
    newTri1 = tris.size()-3;
    newTri1Node = new DagNode(newTri1);
    oldTri->addChild(newTri1Node);
    activeList.push_back(true);

    //triangle 2
    tris.push_back(newPointIndex);
    tris.push_back(tris[i+1]);
    tris.push_back(tris[i+2]);
    newTri2 = tris.size()-3;
    newTri2Node = new DagNode(newTri2);
    oldTri->addChild(newTri2Node);
    activeList.push_back(true);


    //triangle 3
    tris.push_back(tris[i]);
    tris.push_back(newPointIndex);
    tris.push_back(tris[i+2]);
    newTri3 = tris.size()-3;
    newTri3Node = new DagNode(newTri3);
    oldTri->addChild(newTri3Node);
    activeList.push_back(true);

    int current = tris.size();

    //adjacencies of the first triangle

    adj.push_back(adj[i]); //along edge 01->parentAdj along 01, 12->triangle 2, 20->triangle 3
    adj.push_back(oldTri->getChildren()[1]);
    adj.push_back(oldTri->getChildren()[2]);

    int adjTri = adj[i]->getTIndex();

    if (tris[adjTri] != tris[newTri1] && tris[adjTri] != tris[newTri1+1]) adj[adjTri+1] = newTri1Node;
    if (tris[adjTri+1] != tris[newTri1] && tris[adjTri+1] != tris[newTri1+1]) adj[adjTri+2] = newTri1Node;
    if (tris[adjTri+2] != tris[newTri1] && tris[adjTri+2] != tris[newTri1+1]) adj[adjTri] = newTri1Node;


    //adjacencies of the second triangle

    adj.push_back(oldTri->getChildren()[0]); //along edge 01->triangle 1, 12->parentAdj along 12 20->triangle 3
    adj.push_back(adj[i+1]);
    adj.push_back(oldTri->getChildren()[2]);

    adjTri = adj[i+1]->getTIndex();

    if (tris[adjTri] != tris[newTri2+1] && tris[adjTri] != tris[newTri2+2]) adj[adjTri+1] = newTri2Node;
    if (tris[adjTri+1] != tris[newTri2+1] && tris[adjTri+1] != tris[newTri2+2]) adj[adjTri+2] = newTri2Node;
    if (tris[adjTri+2] != tris[newTri2+1] && tris[adjTri+2] != tris[newTri2+2]) adj[adjTri] = newTri2Node;


    //adjacencies of the third triangle

    adj.push_back(oldTri->getChildren()[0]); //along edge 01->triangle 1, 12->triangle 2, 20->parentAdj along 20
    adj.push_back(oldTri->getChildren()[1]);
    adj.push_back(adj[i+2]);

    adjTri = adj[i+2]->getTIndex();

    if (tris[adjTri] != tris[newTri3] && tris[adjTri] != tris[newTri3+2]) adj[adjTri+1] = newTri3Node;
    if (tris[adjTri+1] != tris[newTri3] && tris[adjTri+1] != tris[newTri3+2]) adj[adjTri+2] = newTri3Node;
    if (tris[adjTri+2] != tris[newTri3] && tris[adjTri+2] != tris[newTri3+2]) adj[adjTri] = newTri3Node;


    /*
    if (adj[(tris.size()-9)+0]->getTIndex() != 0) legalizeEdge(tris.size()-9, 0);
    if (adj[(tris.size()-6)+1]->getTIndex() != 0) legalizeEdge(tris.size()-6, 1);
    if (adj[(tris.size()-3)+2]->getTIndex() != 0) legalizeEdge(tris.size()-3, 2);
    */
    legalizeEdge(tris.size()-9, 0);
    legalizeEdge(tris.size()-6, 1);
    legalizeEdge(tris.size()-3, 2);
}

std::vector<Point2Dd> Triangulation::getVertices() const
{
    return vertices;
}

void Triangulation::legalizeEdge(int tri, int edge)
{

    DagNode* adjTriNode = adj[tri+edge];
    int adjTri = adjTriNode->getTIndex();
    int oppositeVertex;
    int vertex1, vertex2, vertexNotEdge, adjAdj, adjTriToUpdate;

    switch(edge)
    {

    case 0:

        vertex1 = tris[tri];
        vertex2 = tris[tri+1];
        vertexNotEdge = tris[tri+2];

        if (tris[adjTri] != vertex1 && tris[adjTri] != vertex2)
        {
            oppositeVertex = tris[adjTri];
            adjAdj = 1;
        }
        if (tris[adjTri+1] != vertex1 && tris[adjTri+1] != vertex2)
        {
            oppositeVertex = tris[adjTri+1];
            adjAdj = 2;
        }
        if (tris[adjTri+2] != vertex1 && tris[adjTri+2] != vertex2)
        {
            oppositeVertex = tris[adjTri+2];
            adjAdj = 0;
        }

        if (DelaunayTriangulation::Checker::isPointLyingInCircle(vertices[vertex1], vertices[vertex2], vertices[vertexNotEdge], vertices[oppositeVertex], false))
        {
            /*FLIP*/
            qDebug() << "Flipping edge: " << vertex1 << " - " << vertex2 << ", opposite vertex is: " << oppositeVertex;


            //deactivate the two triangles involved
            activeList[tri/3] = false;
            activeList[adj[tri+edge]->getTIndex()/3] = false;

            //New Triangle 1
            tris.push_back(vertex1);
            tris.push_back(oppositeVertex);
            tris.push_back(vertexNotEdge);
            DagNode* tri1Node = new DagNode (tris.size()-3);
            activeList.push_back(true);

            //New Triangle 2
            tris.push_back(oppositeVertex);
            tris.push_back(vertex2);
            tris.push_back(vertexNotEdge);
            DagNode* tri2Node = new DagNode(tris.size()-3);
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
            int incidentTri1 = adj[adjTri+((adjAdj+1)%3)]->getTIndex(); //tri adjacent along first edge
            int startingVertex1; //will store along what edge of the incident tri tri1 lies on

            if (tris[incidentTri1] !=  vertex1 && tris[incidentTri1] != oppositeVertex) startingVertex1 = 1;
            if (tris[incidentTri1+1] !=  vertex1 && tris[incidentTri1+1] != oppositeVertex) startingVertex1 = 2;
            if (tris[incidentTri1+2] !=  vertex1 && tris[incidentTri1+2] != oppositeVertex) startingVertex1 = 0;

            adj[incidentTri1+startingVertex1] = tri1Node;

            int startingVertex2, incidentTri2;
            incidentTri2 = adj[tri+2]->getTIndex();
            if (tris[incidentTri2] !=  vertexNotEdge && tris[incidentTri2] != vertex1) startingVertex2 = 1;
            if (tris[incidentTri2+1] !=  vertexNotEdge && tris[incidentTri2+1] != vertex1) startingVertex2 = 2;
            if (tris[incidentTri2+2] !=  vertexNotEdge && tris[incidentTri2+2] != vertex1) startingVertex2 = 0;

            adj[incidentTri2+startingVertex2] = tri1Node;


            //Tris incident to tri2

            int startingVertex3, incidentTri3;
            incidentTri3 = adj[adjTri+((adjAdj+2)%3)]->getTIndex();
            if (tris[incidentTri3] !=  vertex2 && tris[incidentTri3] != oppositeVertex) startingVertex3 = 1;
            if (tris[incidentTri3+1] !=  vertex2 && tris[incidentTri3+1] != oppositeVertex) startingVertex3 = 2;
            if (tris[incidentTri3+2] !=  vertex2 && tris[incidentTri3+2] != oppositeVertex) startingVertex3 = 0;

            adj[incidentTri3+startingVertex3] = tri2Node;


            int startingVertex4, incidentTri4;
            incidentTri4 = adj[tri+1]->getTIndex();
            if (tris[incidentTri4] !=  vertexNotEdge && tris[incidentTri4] != oppositeVertex) startingVertex4 = 1;
            if (tris[incidentTri4+1] !=  vertexNotEdge && tris[incidentTri4+1] != oppositeVertex) startingVertex4 = 2;
            if (tris[incidentTri4+2] !=  vertexNotEdge && tris[incidentTri4+2] != oppositeVertex) startingVertex4 = 0;

            adj[incidentTri4+startingVertex4] = tri2Node;

            if (adj[tris.size()-6]->getTIndex() != 0) legalizeEdge(tris.size()-6, 0);
            if (adj[tris.size()-6]->getTIndex() != 0) legalizeEdge(tris.size()-3, 0);

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
        if (tris[adjTri+1] != vertex1 && tris[adjTri+1] != vertex2)
        {
            oppositeVertex = tris[adjTri+1];
            adjAdj = 2;
        }
        if (tris[adjTri+2] != vertex1 && tris[adjTri+2] != vertex2)
        {
            oppositeVertex = tris[adjTri+2];
            adjAdj = 0;
        }

        if (DelaunayTriangulation::Checker::isPointLyingInCircle(vertices[vertex1], vertices[vertex2], vertices[vertexNotEdge], vertices[oppositeVertex], false))
        {
            /*FLIP*/

            qDebug() << "Flipping edge: " << vertex1 << " - " << vertex2 << ", opposite vertex is: " << oppositeVertex;

            //deactivate the two triangles involved
            activeList[tri/3] = false;
            activeList[adj[tri+edge]->getTIndex()/3] = false;

            //New Triangle 1
            tris.push_back(vertex1);
            tris.push_back(oppositeVertex);
            tris.push_back(vertexNotEdge);
            DagNode* tri1Node = new DagNode (tris.size()-3);
            activeList.push_back(true);

            //New Triangle 2
            tris.push_back(oppositeVertex);
            tris.push_back(vertex2);
            tris.push_back(vertexNotEdge);
            DagNode* tri2Node = new DagNode(tris.size()-3);
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
            int incidentTri1 = adj[adjTri+((adjAdj+1)%3)]->getTIndex(); //tri adjacent along first edge
            int startingVertex1; //will store along what edge of the incident tri tri1 lies on

            if (tris[incidentTri1] !=  vertex1 && tris[incidentTri1] != oppositeVertex) startingVertex1 = 1;
            if (tris[incidentTri1+1] !=  vertex1 && tris[incidentTri1+1] != oppositeVertex) startingVertex1 = 2;
            if (tris[incidentTri1+2] !=  vertex1 && tris[incidentTri1+2] != oppositeVertex) startingVertex1 = 0;

            adj[incidentTri1+startingVertex1] = tri1Node;

            int startingVertex2, incidentTri2;
            incidentTri2 = adj[tri]->getTIndex();
            if (tris[incidentTri2] !=  vertexNotEdge && tris[incidentTri2] != vertex1) startingVertex2 = 1;
            if (tris[incidentTri2+1] !=  vertexNotEdge && tris[incidentTri2+1] != vertex1) startingVertex2 = 2;
            if (tris[incidentTri2+2] !=  vertexNotEdge && tris[incidentTri2+2] != vertex1) startingVertex2 = 0;

            adj[incidentTri2+startingVertex2] = tri1Node;


            //Tris incident to tri2

            int startingVertex3, incidentTri3;
            incidentTri3 = adj[adjTri+((adjAdj+2)%3)]->getTIndex();
            if (tris[incidentTri3] !=  vertex2 && tris[incidentTri3] != oppositeVertex) startingVertex3 = 1;
            if (tris[incidentTri3+1] !=  vertex2 && tris[incidentTri3+1] != oppositeVertex) startingVertex3 = 2;
            if (tris[incidentTri3+2] !=  vertex2 && tris[incidentTri3+2] != oppositeVertex) startingVertex3 = 0;

            adj[incidentTri3+startingVertex3] = tri2Node;


            int startingVertex4, incidentTri4;
            incidentTri4 = adj[tri+2]->getTIndex();
            if (tris[incidentTri4] !=  vertexNotEdge && tris[incidentTri4] != oppositeVertex) startingVertex4 = 1;
            if (tris[incidentTri4+1] !=  vertexNotEdge && tris[incidentTri4+1] != oppositeVertex) startingVertex4 = 2;
            if (tris[incidentTri4+2] !=  vertexNotEdge && tris[incidentTri4+2] != oppositeVertex) startingVertex4 = 0;

            adj[incidentTri4+startingVertex4] = tri2Node;

            if (adj[tris.size()-6]->getTIndex() != 0) legalizeEdge(tris.size()-6, 0);
            if (adj[tris.size()-6]->getTIndex() != 0) legalizeEdge(tris.size()-3, 0);

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
        if (tris[adjTri+1] != vertex1 && tris[adjTri+1] != vertex2)
        {
            oppositeVertex = tris[adjTri+1];
            adjAdj = 2;
        }
        if (tris[adjTri+2] != vertex1 && tris[adjTri+2] != vertex2)
        {
            oppositeVertex = tris[adjTri+2];
            adjAdj = 0;
        }

        if (DelaunayTriangulation::Checker::isPointLyingInCircle(vertices[vertex1], vertices[vertex2], vertices[vertexNotEdge], vertices[oppositeVertex], false))
        {
            /*FLIP*/

            qDebug() << "Flipping edge: " << vertex1 << " - " << vertex2 << ", opposite vertex is: " << oppositeVertex;


            //deactivate the two triangles involved
            activeList[tri/3] = false;
            activeList[adj[tri+edge]->getTIndex()/3] = false;


            //New Triangle 1
            tris.push_back(vertex1);
            tris.push_back(oppositeVertex);
            tris.push_back(vertexNotEdge);
            DagNode* tri1Node = new DagNode (tris.size()-3);
            activeList.push_back(true);

            //New Triangle 2
            tris.push_back(oppositeVertex);
            tris.push_back(vertex2);
            tris.push_back(vertexNotEdge);
            DagNode* tri2Node = new DagNode(tris.size()-3);
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
            int incidentTri1 = adj[adjTri+((adjAdj+1)%3)]->getTIndex(); //tri adjacent along first edge
            int startingVertex1; //will store along what edge of the incident tri tri1 lies on

            if (tris[incidentTri1] !=  vertex1 && tris[incidentTri1] != oppositeVertex) startingVertex1 = 1;
            if (tris[incidentTri1+1] !=  vertex1 && tris[incidentTri1+1] != oppositeVertex) startingVertex1 = 2;
            if (tris[incidentTri1+2] !=  vertex1 && tris[incidentTri1+2] != oppositeVertex) startingVertex1 = 0;

            adj[incidentTri1+startingVertex1] = tri1Node;

            int startingVertex2, incidentTri2;
            incidentTri2 = adj[tri+1]->getTIndex();
            if (tris[incidentTri2] !=  vertexNotEdge && tris[incidentTri2] != vertex1) startingVertex2 = 1;
            if (tris[incidentTri2+1] !=  vertexNotEdge && tris[incidentTri2+1] != vertex1) startingVertex2 = 2;
            if (tris[incidentTri2+2] !=  vertexNotEdge && tris[incidentTri2+2] != vertex1) startingVertex2 = 0;

            adj[incidentTri2+startingVertex2] = tri1Node;


            //Tris incident to tri2

            int startingVertex3, incidentTri3;
            incidentTri3 = adj[adjTri+((adjAdj+2)%3)]->getTIndex();
            if (tris[incidentTri3] !=  vertex2 && tris[incidentTri3] != oppositeVertex) startingVertex3 = 1;
            if (tris[incidentTri3+1] !=  vertex2 && tris[incidentTri3+1] != oppositeVertex) startingVertex3 = 2;
            if (tris[incidentTri3+2] !=  vertex2 && tris[incidentTri3+2] != oppositeVertex) startingVertex3 = 0;

            adj[incidentTri3+startingVertex3] = tri2Node;


            int startingVertex4, incidentTri4;
            incidentTri4 = adj[tri]->getTIndex();
            if (tris[incidentTri4] !=  vertexNotEdge && tris[incidentTri4] != oppositeVertex) startingVertex4 = 1;
            if (tris[incidentTri4+1] !=  vertexNotEdge && tris[incidentTri4+1] != oppositeVertex) startingVertex4 = 2;
            if (tris[incidentTri4+2] !=  vertexNotEdge && tris[incidentTri4+2] != oppositeVertex) startingVertex4 = 0;

            adj[incidentTri4+startingVertex4] = tri2Node;

            if (adj[tris.size()-6]->getTIndex() != 0) legalizeEdge(tris.size()-6, 0);
            if (adj[tris.size()-6]->getTIndex() != 0) legalizeEdge(tris.size()-3, 0);

        }
        break;



    }










}
std::vector<uint> Triangulation::getTris() const
{
    return tris;
}

std::vector<DagNode*> Triangulation::getAdj() const
{
    return adj;
}
