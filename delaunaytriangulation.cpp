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

void Triangulation::clearTriangulation()
{
    vertices.resize(3); //resize also destroys elements, so it should destroy each Point2Dd correctly
    tris.resize(3);

    for (DagNode* node : adj)
        node = nullptr;

    adj.clear();
    adj.push_back(dag->getRoot());
    adj.push_back(dag->getRoot());
    adj.push_back(dag->getRoot());
    activeList.resize(1);

    //Clearing Dag

    dag->clearDag();
}


int Triangulation::findOppositeVertex(int v1, int v2, int adjTri, int &edgeTriIsAdjacentOn)
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

int Triangulation::findOppositeVertex(int v1, int v2, int adjTri)
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

void Triangulation::addPoint(Point2Dd newPoint)
{
    DagNode* oldTri = dag->locate(newPoint, this);
    uint oldTriIndex = oldTri->getTIndex();

    Point2Dd a = vertices[tris[oldTriIndex]];
    Point2Dd b = vertices[tris[oldTriIndex+1]];
    Point2Dd c = vertices[tris[oldTriIndex+2]];

    if (newPoint == a || newPoint == b || newPoint == c) return;

    /*
    if (geomUtils::isPointOnLine(newPoint, a, b) ||
        geomUtils::isPointOnLine(newPoint, b, c) ||
        geomUtils::isPointOnLine(newPoint, c, a)
        )
    {



        DagNode* otherTri;
        int otherTriIndex;
        vertices.push_back(newPoint);
        int newPointIndex = vertices.size()-1;
        DagNode* newTri1Node;
        DagNode* newTri2Node;
        DagNode* newTri3Node;
        DagNode* newTri4Node;
        int newTri1, newTri2, newTri3, newTri4;
        int oppositeVertex;
        int edgeTriIsAdjacentOn;
        int edgeOtherTriIsAdjacentOn;

        if (geomUtils::isPointOnLine(newPoint, a, b))
        {
             otherTri = adj[oldTriIndex];
             otherTriIndex = otherTri->getTIndex();
             oppositeVertex = findOppositeVertex(tris[oldTriIndex], tris[oldTriIndex+1], otherTriIndex, edgeTriIsAdjacentOn);
             edgeOtherTriIsAdjacentOn = 0;
        }
        else if (geomUtils::isPointOnLine(newPoint, b, c))
            {
                otherTri = adj[oldTriIndex+1];
                otherTriIndex = otherTri->getTIndex();
                oppositeVertex = findOppositeVertex(tris[oldTriIndex+1], tris[oldTriIndex+2], otherTriIndex, edgeTriIsAdjacentOn);
                edgeOtherTriIsAdjacentOn = 1;
            }
            else
            {
                otherTri = adj[oldTriIndex+2];
                otherTriIndex = otherTri->getTIndex();
                oppositeVertex = findOppositeVertex(tris[oldTriIndex+2], tris[oldTriIndex], otherTriIndex, edgeTriIsAdjacentOn);
                edgeOtherTriIsAdjacentOn = 2;
            }



        //triangles being split are not active anymore
        activeList[oldTriIndex/3] = false;
        activeList[otherTriIndex/3] = false;


        switch(edgeOtherTriIsAdjacentOn)
        case 0:

            //triangle 1
            tris.push_back(newPointIndex);
            tris.push_back(tris[oldTriIndex+((2+edgeOtherTriIsAdjacentOn)%3)]);
            tris.push_back(tris[oldTriIndex+((edgeOtherTriIsAdjacentOn)%3)]);
            newTri1 = tris.size()-3;
            newTri1Node = new DagNode(newTri1);
            dag->addNode(newTri1Node);
            oldTri->addChild(newTri1Node);
            activeList.push_back(true);

            //triangle 2
            tris.push_back(newPointIndex);
            tris.push_back(tris[oldTriIndex+((edgeOtherTriIsAdjacentOn)%3)]);
            tris.push_back(oppositeVertex);
            newTri2 = tris.size()-3;
            newTri2Node = new DagNode(newTri2);
            dag->addNode(newTri2Node);
            otherTri->addChild(newTri2Node);
            activeList.push_back(true);

            //triangle 3
            tris.push_back(newPointIndex);
            tris.push_back(oppositeVertex);
            tris.push_back(tris[oldTriIndex+((1+edgeOtherTriIsAdjacentOn)%3)]);
            newTri3 = tris.size()-3;
            newTri3Node = new DagNode(newTri3);
            dag->addNode(newTri3Node);
            otherTri->addChild(newTri3Node);
            activeList.push_back(true);

            //triangle 4
            tris.push_back(newPointIndex);
            tris.push_back(tris[oldTriIndex+((1+edgeOtherTriIsAdjacentOn)%3)]);
            tris.push_back(tris[oldTriIndex+((2+edgeOtherTriIsAdjacentOn)%3)]);
            newTri4 = tris.size()-3;
            newTri4Node = new DagNode(newTri4);
            dag->addNode(newTri4Node);
            oldTri->addChild(newTri4Node);
            activeList.push_back(true);

            //adjacencies of the first triangle

            adj.push_back(newTri4Node); //along first edge
            adj.push_back(adj[oldTriIndex+((2+edgeOtherTriIsAdjacentOn)%3)]); //along second edge
            adj.push_back(newTri2Node); //along third edge

            int adjTri = adj[oldTriIndex+((2+edgeOtherTriIsAdjacentOn)%3)]->getTIndex(); // refreshing the adjacent triangle's adjacency that changed

            if (adjTri != 0)
            {
                if (tris[adjTri] != tris[newTri1] && tris[adjTri] != tris[newTri1+1]) adj[adjTri+1] = newTri1Node;
                else if (tris[adjTri+1] != tris[newTri1] && tris[adjTri+1] != tris[newTri1+1]) adj[adjTri+2] = newTri1Node;
                else adj[adjTri] = newTri1Node;
            }


            //adjacencies of the second triangle

            adj.push_back(newTri1Node); //along first edge
            adj.push_back(adj[otherTriIndex+((edgeTriIsAdjacentOn+1)%3)]); //along second edge
            adj.push_back(newTri3Node); //along third edge

            adjTri = adj[otherTriIndex+((edgeTriIsAdjacentOn+1)%3)]->getTIndex(); // refreshing the adjacent triangle's adjacency that changed

            if (adjTri != 0)
            {
                if (tris[adjTri] != tris[newTri1] && tris[adjTri] != tris[newTri1+1]) adj[adjTri+1] = newTri2Node;
                else if (tris[adjTri+1] != tris[newTri1] && tris[adjTri+1] != tris[newTri1+1]) adj[adjTri+2] = newTri2Node;
                else adj[adjTri] = newTri2Node;
            }

            //adjacencies of the third triangle

            adj.push_back(newTri2Node); //along first edge
            adj.push_back(adj[otherTriIndex+((edgeTriIsAdjacentOn+2)%3)]); //along second edge
            adj.push_back(newTri4Node); //along third edge

            adjTri = adj[otherTriIndex+((edgeTriIsAdjacentOn+2)%3)]->getTIndex(); // refreshing the adjacent triangle's adjacency that changed

            if (adjTri != 0)
            {
                if (tris[adjTri] != tris[newTri1] && tris[adjTri] != tris[newTri1+1]) adj[adjTri+1] = newTri3Node;
                else if (tris[adjTri+1] != tris[newTri1] && tris[adjTri+1] != tris[newTri1+1]) adj[adjTri+2] = newTri3Node;
                else adj[adjTri] = newTri3Node;
            }

            //adjacencies of the fourth triangle

            adj.push_back(newTri3Node); //along first edge
            adj.push_back(adj[oldTriIndex+((1+edgeOtherTriIsAdjacentOn)%3)]); //along second edge
            adj.push_back(newTri1Node); //along third edge

            adjTri = adj[oldTriIndex+((1+edgeOtherTriIsAdjacentOn)%3)]->getTIndex(); // refreshing the adjacent triangle's adjacency that changed

            if (adjTri != 0)
            {
                if (tris[adjTri] != tris[newTri1] && tris[adjTri] != tris[newTri1+1]) adj[adjTri+1] = newTri4Node;
                else if (tris[adjTri+1] != tris[newTri1] && tris[adjTri+1] != tris[newTri1+1]) adj[adjTri+2] = newTri4Node;
                else adj[adjTri] = newTri4Node;
            }


            legalizeEdge(newTri1, 1);
            legalizeEdge(newTri2, 1);
            legalizeEdge(newTri3, 1);
            legalizeEdge(newTri4, 1);





        case 1:

            //triangle 1
            tris.push_back(newPointIndex);
            tris.push_back(tris[oldTriIndex]);
            tris.push_back(tris[oldTriIndex+1]);
            newTri1 = tris.size()-3;
            newTri1Node = new DagNode(newTri1);
            dag->addNode(newTri1Node);
            oldTri->addChild(newTri1Node);
            activeList.push_back(true);


            //triangle 2
            tris.push_back(newPointIndex);
            tris.push_back(tris[oldTriIndex+1]);
            tris.push_back(oppositeVertex);
            newTri2 = tris.size()-3;
            newTri2Node = new DagNode(newTri2);
            dag->addNode(newTri2Node);
            otherTri->addChild(newTri2Node);
            activeList.push_back(true);

            //triangle 3
            tris.push_back(newPointIndex);
            tris.push_back(oppositeVertex);
            tris.push_back(tris[oldTriIndex+2]);
            newTri3 = tris.size()-3;
            newTri3Node = new DagNode(newTri3);
            dag->addNode(newTri3Node);
            otherTri->addChild(newTri3Node);
            activeList.push_back(true);

            //triangle 4
            tris.push_back(newPointIndex);
            tris.push_back(tris[oldTriIndex+2]);
            tris.push_back(tris[oldTriIndex]);
            newTri4 = tris.size()-3;
            newTri4Node = new DagNode(newTri4);
            dag->addNode(newTri4Node);
            oldTri->addChild(newTri4Node);
            activeList.push_back(true);

            //adjacencies of the first triangle

            adj.push_back(newTri4Node); //along first edge
            adj.push_back(adj[oldTriIndex]); //along second edge
            adj.push_back(newTri2Node); //along third edge

            int adjTri = adj[oldTriIndex]->getTIndex(); // refreshing the adjacent triangle's adjacency that changed

            if (adjTri != 0)
            {
                if (tris[adjTri] != tris[newTri1] && tris[adjTri] != tris[newTri1+1]) adj[adjTri+1] = newTri1Node;
                else if (tris[adjTri+1] != tris[newTri1] && tris[adjTri+1] != tris[newTri1+1]) adj[adjTri+2] = newTri1Node;
                else adj[adjTri] = newTri1Node;
            }


            //adjacencies of the second triangle

            adj.push_back(newTri1Node); //along first edge
            adj.push_back(otherTriIndex+((edgeTriIsAdjacentOn+1)%3)); //along second edge
            adj.push_back(newTri3Node); //along third edge

            int adjTri = adj[otherTriIndex+((edgeTriIsAdjacentOn+1)%3)]->getTIndex(); // refreshing the adjacent triangle's adjacency that changed

            if (adjTri != 0)
            {
                if (tris[adjTri] != tris[newTri1] && tris[adjTri] != tris[newTri1+1]) adj[adjTri+1] = newTri2Node;
                else if (tris[adjTri+1] != tris[newTri1] && tris[adjTri+1] != tris[newTri1+1]) adj[adjTri+2] = newTri2Node;
                else adj[adjTri] = newTri2Node;
            }

            //adjacencies of the third triangle

            adj.push_back(newTri2Node); //along first edge
            adj.push_back(otherTriIndex+((edgeTriIsAdjacentOn+2)%3)); //along second edge
            adj.push_back(newTri4Node]); //along third edge

            int adjTri = adj[otherTriIndex+((edgeTriIsAdjacentOn+2)%3)]->getTIndex(); // refreshing the adjacent triangle's adjacency that changed

            if (adjTri != 0)
            {
                if (tris[adjTri] != tris[newTri1] && tris[adjTri] != tris[newTri1+1]) adj[adjTri+1] = newTri3Node;
                else if (tris[adjTri+1] != tris[newTri1] && tris[adjTri+1] != tris[newTri1+1]) adj[adjTri+2] = newTri3Node;
                else adj[adjTri] = newTri3Node;
            }

            //adjacencies of the fourth triangle

            adj.push_back(newTri3Node); //along first edge
            adj.push_back(adj[oldTriIndex+2]); //along second edge
            adj.push_back(newTri1Node]); //along third edge

            int adjTri = adj[oldTriIndex+2]->getTIndex(); // refreshing the adjacent triangle's adjacency that changed

            if (adjTri != 0)
            {
                if (tris[adjTri] != tris[newTri1] && tris[adjTri] != tris[newTri1+1]) adj[adjTri+1] = newTri4Node;
                else if (tris[adjTri+1] != tris[newTri1] && tris[adjTri+1] != tris[newTri1+1]) adj[adjTri+2] = newTri4Node;
                else adj[adjTri] = newTri4Node;
            }


            legalizeEdge(newTri1, 1);
            legalizeEdge(newTri2, 1);
            legalizeEdge(newTri3, 1);
            legalizeEdge(newTri4, 1);

            break;

    }
    else*/
    {
        vertices.push_back(newPoint);
        int newPointIndex = vertices.size()-1;
        DagNode* newTri1Node;
        DagNode* newTri2Node;
        DagNode* newTri3Node;
        int newTri1, newTri2, newTri3;


        //triangle being split is not active anymore
        activeList[oldTriIndex/3] = false;

        //triangle 1
        tris.push_back(tris[oldTriIndex]);
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

        adj.push_back(adj[oldTriIndex]); //along edge 01->parentAdj along 01, 12->triangle 2, 20->triangle 3
        adj.push_back(newTri2Node);
        adj.push_back(newTri3Node);

        int adjTri = adj[oldTriIndex]->getTIndex();

        if (adjTri != 0)
        {
            if (tris[adjTri] != tris[newTri1] && tris[adjTri] != tris[newTri1+1]) adj[adjTri+1] = newTri1Node;
            if (tris[adjTri+1] != tris[newTri1] && tris[adjTri+1] != tris[newTri1+1]) adj[adjTri+2] = newTri1Node;
            if (tris[adjTri+2] != tris[newTri1] && tris[adjTri+2] != tris[newTri1+1]) adj[adjTri] = newTri1Node;
        }

        //adjacencies of the second triangle

        adj.push_back(newTri1Node); //along edge 01->triangle 1, 12->parentAdj along 12 20->triangle 3
        adj.push_back(adj[oldTriIndex+1]);
        adj.push_back(newTri3Node);

        adjTri = adj[oldTriIndex+1]->getTIndex();

        if (adjTri != 0)
        {
            if (tris[adjTri] != tris[newTri2+1] && tris[adjTri] != tris[newTri2+2]) adj[adjTri+1] = newTri2Node;
            if (tris[adjTri+1] != tris[newTri2+1] && tris[adjTri+1] != tris[newTri2+2]) adj[adjTri+2] = newTri2Node;
            if (tris[adjTri+2] != tris[newTri2+1] && tris[adjTri+2] != tris[newTri2+2]) adj[adjTri] = newTri2Node;
        }

        //adjacencies of the third triangle

        adj.push_back(newTri1Node); //along edge 01->triangle 1, 12->triangle 2, 20->parentAdj along 20
        adj.push_back(newTri2Node);
        adj.push_back(adj[oldTriIndex+2]);

        adjTri = adj[oldTriIndex+2]->getTIndex();

        if (adjTri != 0)
        {
            if (tris[adjTri] != tris[newTri3] && tris[adjTri] != tris[newTri3+2]) adj[adjTri+1] = newTri3Node;
            if (tris[adjTri+1] != tris[newTri3] && tris[adjTri+1] != tris[newTri3+2]) adj[adjTri+2] = newTri3Node;
            if (tris[adjTri+2] != tris[newTri3] && tris[adjTri+2] != tris[newTri3+2]) adj[adjTri] = newTri3Node;
        }


        //if (adj[(tris.size()-9)+0]->getTIndex() != 0) legalizeEdge(tris.size()-9, 0);
        //if (adj[(tris.size()-6)+1]->getTIndex() != 0) legalizeEdge(tris.size()-6, 1);
        //if (adj[(tris.size()-3)+2]->getTIndex() != 0) legalizeEdge(tris.size()-3, 2);



        legalizeEdge(newTri1, 0);
        legalizeEdge(newTri2, 1);
        legalizeEdge(newTri3, 2);


    }



}

std::vector<Point2Dd> &Triangulation::getVertices()
{
    return vertices;
}

void Triangulation::legalizeEdge(int tri, int edge)
{

    DagNode* adjTriNode = adj[tri+edge];
    int adjTri = adjTriNode->getTIndex();
    int oppositeVertex;
    int vertex1, vertex2, vertexNotEdge, adjAdj;


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

        if (geomUtils::isPointOnLine(vertices[vertexNotEdge], vertices[vertex1], vertices[vertex2]) ||
            DelaunayTriangulation::Checker::isPointLyingInCircle(vertices[vertex1], vertices[vertex2], vertices[vertexNotEdge], vertices[oppositeVertex], false))
        {
            /*FLIP*/
            if (geomUtils::isPointOnLine(vertices[vertexNotEdge], vertices[vertex1], vertices[vertex2])) qDebug() << "It works!";


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
            if (tris[incidentTri4] !=  vertexNotEdge && tris[incidentTri4] != vertex2) startingVertex4 = 1;
            if (tris[incidentTri4+1] !=  vertexNotEdge && tris[incidentTri4+1] != vertex2) startingVertex4 = 2;
            if (tris[incidentTri4+2] !=  vertexNotEdge && tris[incidentTri4+2] != vertex2) startingVertex4 = 0;

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

        if (geomUtils::isPointOnLine(vertices[vertexNotEdge], vertices[vertex1], vertices[vertex2]) ||
            DelaunayTriangulation::Checker::isPointLyingInCircle(vertices[vertex1], vertices[vertex2], vertices[vertexNotEdge], vertices[oppositeVertex], false))
        {
            /*FLIP*/
            if (geomUtils::isPointOnLine(vertices[vertexNotEdge], vertices[vertex1], vertices[vertex2])) qDebug() << "It works!";


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
            if (tris[incidentTri4] !=  vertexNotEdge && tris[incidentTri4] != vertex2) startingVertex4 = 1;
            if (tris[incidentTri4+1] !=  vertexNotEdge && tris[incidentTri4+1] != vertex2) startingVertex4 = 2;
            if (tris[incidentTri4+2] !=  vertexNotEdge && tris[incidentTri4+2] != vertex2) startingVertex4 = 0;

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

        if (geomUtils::isPointOnLine(vertices[vertexNotEdge], vertices[vertex1], vertices[vertex2]) ||
            DelaunayTriangulation::Checker::isPointLyingInCircle(vertices[vertex1], vertices[vertex2], vertices[vertexNotEdge], vertices[oppositeVertex], false))
        {
            /*FLIP*/
            if (geomUtils::isPointOnLine(vertices[vertexNotEdge], vertices[vertex1], vertices[vertex2])) qDebug() << "It works!";


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
            if (tris[incidentTri4] !=  vertexNotEdge && tris[incidentTri4] != vertex2) startingVertex4 = 1;
            if (tris[incidentTri4+1] !=  vertexNotEdge && tris[incidentTri4+1] != vertex2) startingVertex4 = 2;
            if (tris[incidentTri4+2] !=  vertexNotEdge && tris[incidentTri4+2] != vertex2) startingVertex4 = 0;

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
