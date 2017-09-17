#include "triangle.h"


inline Triangle::Triangle(): a(0, std::numeric_limits<double>::max()), b(-std::numeric_limits<double>::max(), -std::numeric_limits<double>::max()), c(std::numeric_limits<double>::max(), -std::numeric_limits<double>::max())
{

}

inline Triangle::Triangle(const Point2Dd &a, const Point2Dd &b, const Point2Dd &c): a(a), b(b), c(c)
{

}

inline Point2Dd Triangle::center() const
{

    return Point2Dd(( a.x() + b.x() + c.x() ) / 3 ,( a.y() + b.y() + c.y() ) / 3); // return centroid of triangle

}

inline double Triangle::radius() const
{
    return b.dist(Point2Dd(c.x(), a.y()));
}

inline Point2Dd Triangle::getB() const
{
    return b;
}

inline Point2Dd Triangle::getA() const
{
    return a;
}

inline Point2Dd Triangle::getC() const
{
    return c;
}
