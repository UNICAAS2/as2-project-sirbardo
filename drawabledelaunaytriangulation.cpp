#include "drawabledelaunaytriangulation.h"

DrawableDelaunayTriangulation::DrawableDelaunayTriangulation(Triangle a, double thirdCoord):Triangulation(a), pointSize(2), colorPoint(), edgeWidth(1), colorEdge(), thirdCoord(thirdCoord)
{

}

void DrawableDelaunayTriangulation::draw() const {
    drawEdges();
    drawPoints();
}

Pointd DrawableDelaunayTriangulation::sceneCenter() const {
    return Pointd(0, 0, 0);
}

double DrawableDelaunayTriangulation::sceneRadius() const {
    return 1e+6; //placeholder of a proper sceneradius. This is the bounding box size
}

void DrawableDelaunayTriangulation::drawEdges() const {

    if (tris.size() != 3)
    {


        for (int i=3; i<tris.size(); i+=3) //I don't want to draw edges that go to the bounding triangle.
        {
            if (activeList[i/3] == true && tris[i] != 0 && tris[i] != 1 && tris[i] != 2 &&
                tris[i+1] != 0 && tris[i+1] != 1 && tris[i+1] != 2 &&
                tris[i+2] != 0 && tris[i+2] != 1 && tris[i+2] != 2)
            {
                Viewer::drawLine(Pointd(vertices[tris[i]].x(), vertices[tris[i]].y(), thirdCoord), Pointd(vertices[tris[i+1]].x(), vertices[tris[i+1]].y(), thirdCoord), colorEdge, edgeWidth);
                Viewer::drawLine(Pointd(vertices[tris[i+1]].x(), vertices[tris[i+1]].y(), thirdCoord), Pointd(vertices[tris[i+2]].x(), vertices[tris[i+2]].y(), thirdCoord), colorEdge, edgeWidth);
                Viewer::drawLine(Pointd(vertices[tris[i+2]].x(), vertices[tris[i+2]].y(), thirdCoord), Pointd(vertices[tris[i]].x(), vertices[tris[i]].y(), thirdCoord), colorEdge, edgeWidth);
            }
        }

    }
}

void DrawableDelaunayTriangulation::drawPoints() const {
    glEnable(GL_POINT_SMOOTH);
    glPointSize(pointSize);
    glBegin(GL_POINTS);

    glColor3f(colorPoint.redF(), colorPoint.greenF(), colorPoint.blueF());

    for (Point2Dd i: vertices)
    {
        glVertex3d(i.x(), i.y(), thirdCoord);
    }

    glEnd();
}
