#ifndef DRAWABLEVORONOI_H
#define DRAWABLEVORONOI_H

#include <voronoi.h>
#include <delaunaytriangulation.h>
#include <common/color.h>
#include "viewer/interfaces/drawable_object.h"
#include "viewer/objects/objects.h"


class DrawableVoronoi : public DrawableObject, public Voronoi
{

public:
    DrawableVoronoi(Triangulation *t);


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

#endif // DRAWABLEVORONOI_H
