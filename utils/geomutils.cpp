#include "geomutils.h"

/**
 * @brief geomUtils::triContainsPoint, checks if a triangle contains a point
 * @param tri Triangle
 * @param x Point
 * @return true if it does, false if it does not.
 */

bool geomUtils::triContainsPoint(Triangle &tri, Point2Dd &x)
{
    Point2Dd a = tri.getA();
    Point2Dd b = tri.getB();
    Point2Dd c = tri.getC();

    double dX = x.x()-c.x();
    double dY = x.y()-c.y();
    double dX21 = c.x()-b.x();
    double dY12 = b.y()-c.y();


    double D = dY12*(a.x()-c.x()) + dX21*(a.y()-c.y());
    double s = dY12*dX + dX21*dY;
    double t = (c.y()-a.y())*dX + (a.x()-c.x())*dY;

    if (D<0) return s<=0 && t<=0 && s+t>=D;
    return s>=0 && t>=0 && s+t<=D;
}

/**
 * @brief geomUtils::triContainsPoint, checks if a triangle contains a point
 * @param a first vertex of the triangle
 * @param b second vertex of the triangle
 * @param c third vertex of the triangle
 * @param x point
 * @return
 */

bool geomUtils::triContainsPoint(Point2Dd &a, Point2Dd &b, Point2Dd &c, Point2Dd &x)
{

    double dX = x.x()-c.x();
    double dY = x.y()-c.y();
    double dX21 = c.x()-b.x();
    double dY12 = b.y()-c.y();


    double D = dY12*(a.x()-c.x()) + dX21*(a.y()-c.y());
    double s = dY12*dX + dX21*dY;
    double t = (c.y()-a.y())*dX + (a.x()-c.x())*dY;

    if (D<0) return s<=0 && t<=0 && s+t>=D;
    return s>=0 && t>=0 && s+t<=D;


}


/**
 * @brief geomUtils::isPointOnLine, checks if a point lies on a line segment / edge
 * @param currPoint point to check
 * @param point1 endpoint of segment
 * @param point2 endpoint of segment
 * @return
 */
bool geomUtils::isPointOnLine(Point2Dd &currPoint, Point2Dd &point1, Point2Dd &point2)
{
    //https://stackoverflow.com/questions/11907947/how-to-check-if-a-point-lies-on-a-line-between-2-other-points

        double dxc = currPoint.x() - point1.x();
        double dyc = currPoint.y() - point1.y();

        double dxl = point2.x() - point1.x();
        double dyl = point2.y() - point1.y();

        double cross = dxc * dyl - dyc * dxl;

        if (cross != 0)
          return false;


        if (std::abs(dxl) >= std::abs(dyl))
          return dxl > 0 ?
            point1.x() <= currPoint.x() && currPoint.x() <= point2.x() :
            point2.x() <= currPoint.x() && currPoint.x() <= point1.x();

        else
          return dyl > 0 ?
            point1.y() <= currPoint.y() && currPoint.y() <= point2.y() :
            point2.y() <= currPoint.y() && currPoint.y() <= point1.y();


}

/**
 * @brief geomUtils::getCircumcenter
 * Calculates the circumcenter of the given points. My implementation of the formula found here: https://en.wikipedia.org/wiki/Circumscribed_circle
 * @param a
 * @param b
 * @param c
 * @return circumcenter as Point2Dd
 */
Point2Dd geomUtils::getCircumcenter(const Point2Dd a, const Point2Dd b, const Point2Dd c)
{
    double dA, dB, dC;

    dA = pow(a.x(), 2) + pow(a.y(), 2);
    dB = pow(b.x(), 2) + pow(b.y(), 2);
    dC = pow(c.x(), 2) + pow(c.y(), 2);

    double d = 2*( a.x() * ( b.y()-c.y() )    +    b.x() * (c.y()-a.y())   +   c.x()*(a.y()-b.y()) );
    double ux = ( dA*(b.y() - c.y()) + dB*(c.y() - a.y()) + dC*(a.y()-b.y()) ) / d;
    double uy = ( dA*(c.x()-b.x()) + dB*(a.x()-c.x()) + dC*(b.x()-a.x()) ) / d;

    Point2Dd center = Point2Dd (ux, uy);

    return center;

}

