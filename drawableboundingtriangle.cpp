#include "drawableboundingtriangle.h"

DrawableBoundingTriangle::DrawableBoundingTriangle(double thirdCoord) : Triangle(), pointSize(2), colorPoint(), edgeWidth(1), colorEdge(), thirdCoord(thirdCoord)
{
}

DrawableBoundingTriangle::DrawableBoundingTriangle(const Triangle& b, double thirdCoord) : Triangle(b), pointSize(2), colorPoint(), edgeWidth(1), colorEdge(), thirdCoord(thirdCoord)
{
}

DrawableBoundingTriangle::DrawableBoundingTriangle(const Point2Dd& a, const Point2Dd& b, const Point2Dd& c, double thirdCoord) : DrawableBoundingTriangle(Triangle(a, b, c), thirdCoord)
{
}

void DrawableBoundingTriangle::draw() const {
    drawEdges();
    drawPoints();
}

Pointd DrawableBoundingTriangle::sceneCenter() const {
    Point2Dd c = center();
    return Pointd(c.x(), c.y(), 0);
}

double DrawableBoundingTriangle::sceneRadius() const {
    return radius(); //placeholder of a proper sceneradius
}

void DrawableBoundingTriangle::drawEdges() const {
    Viewer::drawLine(Pointd(getB().x(), getB().y(), thirdCoord), Pointd(getA().x(), getA().y(), thirdCoord), colorEdge, edgeWidth);
    Viewer::drawLine(Pointd(getB().x(), getB().y(), thirdCoord), Pointd(getC().x(), getC().y(), thirdCoord), colorEdge, edgeWidth);
    Viewer::drawLine(Pointd(getC().x(), getC().y(), thirdCoord), Pointd(getA().x(), getA().y(), thirdCoord), colorEdge, edgeWidth);
}

void DrawableBoundingTriangle::drawPoints() const {
    glEnable(GL_POINT_SMOOTH);
    glPointSize(pointSize);
    glBegin(GL_POINTS);

    glColor3f(colorPoint.redF(), colorPoint.greenF(), colorPoint.blueF());

    glVertex3d(getB().x(), getB().y(), thirdCoord);
    glVertex3d(getC().x(), getC().y(), thirdCoord);
    glVertex3d(getA().x(), getA().y(), thirdCoord);

    glEnd();
}
