#ifndef SPHERE_H
#define SPHERE_H

#include "object.h"

struct Point3d;
struct ParamLine;

class Sphere : public Object
{
public:
    Sphere(const Point3d& origin, double radius);
    virtual ~Sphere();
    virtual Vector3d getNormal(const Point3d& point) const;
    virtual double getCollision(const ParamLine& line) const;
    virtual void readObject(std::fstream& stream);
    virtual void broadcastObject();
protected:
private:
    Point3d m_origin;
    double m_radius;
};

#endif // SPHERE_H
