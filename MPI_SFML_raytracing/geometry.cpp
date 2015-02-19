#include "geometry.h"
#include <math.h>

Point3d::Point3d()
{
    x=NAN;
    y=NAN;
    z=NAN;
}

Point3d::Point3d(double _x, double _y, double _z)
{
    x=_x;
    y=_y;
    z=_z;
}

Point3d Point3d::operator+(const Point3d& point) const
{
    return Point3d(x+point.x,y+point.y,z+point.z);
}

Point3d Point3d::operator*(const double alpha) const
{
    return Point3d(x*alpha,y*alpha,z*alpha);
}
Point3d Point3d::operator-(const Point3d& point) const
{
    return Point3d(x-point.x,y-point.y,z-point.z);
}

Point3d Point3d::operator/(const double alpha) const
{
    return Point3d(x/alpha,y/alpha,z/alpha);
}

double Point3d::dotProduct(const Vector3d& vec) const
{
    return x*vec.x+y*vec.y+z*vec.z;
}

Vector3d Point3d::crossProduct(const Vector3d& vec) const
{
    return Vector3d(y*vec.z-z*vec.y,x*vec.z-z*vec.x,x*vec.y-y*vec.x);
}
Point3d ParamLine::getPoint(double t)
{
    return origin+vector*t;
}
void ParamLine::normalize()
{
    double dist=0;

    dist = vector.x*vector.x+vector.y*vector.y+vector.z*vector.z;
    dist=sqrt(dist);
    vector=vector/dist;

}

ParamLine::ParamLine()
{

}

ParamLine::ParamLine(Point3d _origin, Vector3d _vector)
{
    origin=_origin;
    vector=_vector;
}
