#ifndef DRAWABLEBOUNDINGTRIANGLE_H
#define DRAWABLEBOUNDINGTRIANGLE_H


#include <triangle.h>
#include <common/color.h>
#include "viewer/interfaces/drawable_object.h"
#include "viewer/objects/objects.h"


class DrawableBoundingTriangle : public Triangle, public DrawableObject
{
    public:
        DrawableBoundingTriangle(double thirdCoord = 0);
        DrawableBoundingTriangle(const Triangle& b, double thirdCoord = 0);
        DrawableBoundingTriangle(const Point2Dd& a, const Point2Dd& b, const Point2Dd& c, double thirdCoord = 0);


        // DrawableObject interface
        void draw() const;
        Pointd sceneCenter() const;
        double sceneRadius() const;

    private:

        void drawEdges() const;
        void drawPoints() const;

        int pointSize;
        Color colorPoint;
        int edgeWidth;
        Color colorEdge;
        double thirdCoord;

};
#endif // DRAWABLEBOUNDINGTRIANGLE_H
