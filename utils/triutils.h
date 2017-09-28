#ifndef TRIUTILS_H
#define TRIUTILS_H

#include "triangle.h"
#include <common/point2d.h>

namespace TriUtils
{

    bool triContainsPoint(Triangle &tri, Point2Dd &x);
    bool triContainsPoint(Point2Dd &a, Point2Dd &b, Point2Dd &c, Point2Dd &x);


}

#endif // TRIUTILS_H
