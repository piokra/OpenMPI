#ifndef GEOMETRY_H

#define GEOMETRY_H


struct Point3d
{
    Point3d();
    Point3d(double x, double y, double z);

    Point3d operator+(const Point3d& point) const;
    Point3d operator*(const double alpha) const;
    Point3d operator-(const Point3d& point) const;
    Point3d operator/(const double alpha) const;

    double dotProduct(const Point3d& vec) const;
    Point3d crossProduct(const Point3d& vec) const;

    double x;
    double y;
    double z;


};

typedef Point3d Vector3d;

struct ParamLine
{
    ParamLine();
    ParamLine(Point3d _origin, Vector3d _vector);
    Point3d origin;
    Vector3d vector;
    Point3d getPoint(double t);
    void normalize();
};



#endif // GEOMETRY_H
