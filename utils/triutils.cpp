#include "triutils.h"


bool TriUtils::triContainsPoint(Triangle tri, Point2Dd x)
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
