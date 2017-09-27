#ifndef DRAWABLEDELAUNAYTRIANGULATION_H
#define DRAWABLEDELAUNAYTRIANGULATION_H

#include <delaunaytriangulation.h>
#include <common/color.h>
#include "viewer/interfaces/drawable_object.h"
#include "viewer/objects/objects.h"


class DrawableDelaunayTriangulation : public Triangulation, public DrawableObject
{
    public:
        DrawableDelaunayTriangulation(Triangle a, double thirdCoord);


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
#endif // DRAWABLEDELAUNAYTRIANGULATION_H
