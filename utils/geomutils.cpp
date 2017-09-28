#include "geomutils.h"


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
