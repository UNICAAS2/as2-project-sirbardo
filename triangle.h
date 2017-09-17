#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "common/point2d.h"

/**
 * @brief The Triangle class
 * This class models a 2D triangle.
 * It is composed of three points, a, b, c, which represent the three
 * points of the triangle.
 */


class Triangle
{
    public:
        Triangle();
        Triangle(const Point2Dd& a, const Point2Dd& b, const Point2Dd& c);
        Point2Dd center() const;
        double radius() const;
        Point2Dd getA() const;
        Point2Dd getB() const;
        Point2Dd getC() const;

    private:
        Point2Dd a, b, c; //Top, Bottom-Left, Bottom-Right

};

#include "triangle.cpp"


#endif // TRIANGLE_H
