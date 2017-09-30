#include "drawablevoronoi.h"

DrawableVoronoi::DrawableVoronoi(Triangulation *t):Voronoi(t)
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


    for (int i = 0; i<vertices.size(); i++)
    {
        qDebug() << "Lol";
        Viewer::drawLine(Pointd(vertices[i].x(), vertices[i].y(), thirdCoord), Pointd(vertices[edges[(i*3)]].x(), vertices[edges[(i*3)]].y(), thirdCoord), colorEdge, edgeWidth);
        Viewer::drawLine(Pointd(vertices[i].x(), vertices[i].y(), thirdCoord), Pointd(vertices[edges[(i*3)+1]].x(), vertices[edges[(i*3)+1]].y(), thirdCoord), colorEdge, edgeWidth);
        Viewer::drawLine(Pointd(vertices[i].x(), vertices[i].y(), thirdCoord), Pointd(vertices[edges[(i*3)+2]].x(), vertices[edges[(i*3)+2]].y(), thirdCoord), colorEdge, edgeWidth);
    }



}

void DrawableVoronoi::drawPoints() const {
    glEnable(GL_POINT_SMOOTH);
    glPointSize(pointSize);
    glBegin(GL_POINTS);

    glColor3f(colorPoint.redF(), colorPoint.greenF(), colorPoint.blueF());

    for (Point2Dd i: vertices)
    {
        qDebug() << i.x() << " " << i.y();
        glVertex3d(i.x(), i.y(), thirdCoord);
    }

    glEnd();
}
