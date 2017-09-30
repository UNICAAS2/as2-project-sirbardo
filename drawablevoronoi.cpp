#include "drawablevoronoi.h"

DrawableVoronoi::DrawableVoronoi(Triangulation *t, double thirdCoord):Voronoi(t), pointSize(2), colorPoint(), edgeWidth(), colorEdge(), thirdCoord(thirdCoord)
{

}

void DrawableVoronoi::draw() const {
    drawEdges();
    drawPoints();
}

Pointd DrawableVoronoi::sceneCenter() const {
    return Pointd(0, 0, 0);
}

double DrawableVoronoi::sceneRadius() const {
    return 1e+6; //placeholder of a proper sceneradius. This is the bounding box size
}

void DrawableVoronoi::drawEdges() const {

    for (int i = 0; i<edges.size(); i+=2)
    {

        Viewer::drawLine(Pointd(vertices[edges[i]].x(), vertices[edges[i]].y(), thirdCoord), Pointd(vertices[edges[i+1]].x(), vertices[edges[i+1]].y(), thirdCoord), colorEdge.blue(), edgeWidth);

    }

}

void DrawableVoronoi::drawPoints() const {

    glEnable(GL_POINT_SMOOTH);
    glPointSize(pointSize);
    glBegin(GL_POINTS);

    glColor3f(colorPoint.redF(), colorPoint.greenF(), colorPoint.blueF());

    for (Point2Dd i: vertices)
    {
        //qDebug() << i.x() << " " << i.y();
        glVertex3d(i.x(), i.y(), thirdCoord);
    }

    glEnd();
}
