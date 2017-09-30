#ifndef GEOMUTILS_H
#define GEOMUTILS_H

#include "triangle.h"
#include <common/point2d.h>
#include <cmath>

/**
 * Algorithms related to geometry calculations.
 */
namespace geomUtils
{

    bool triContainsPoint(Triangle &tri, Point2Dd &x);
    bool triContainsPoint(Point2Dd &a, Point2Dd &b, Point2Dd &c, Point2Dd &x);
    bool isPointOnLine(Point2Dd &currPoint, Point2Dd &point1, Point2Dd &point2);
    Point2Dd getCircumcenter(const Point2Dd a, const Point2Dd b, const Point2Dd c);

}

#endif // GEOMUTILS_H
