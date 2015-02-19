#include "sphere.h"
#include "math.h"
#include "geometry.h"
#include "mpi.h"
#include <iostream>
#include <SFML/Graphics.hpp>
Sphere::Sphere(const Point3d& origin, double radius)
{
    //ctor
    m_origin=origin;
    m_radius=radius;
}

Sphere::~Sphere()
{
    //dtor
}
Vector3d Sphere::getNormal(const Point3d& point) const
{
    return (point-m_origin)/m_radius;
}

double Sphere::getCollision(const ParamLine& line) const
{
    Point3d t = line.origin-m_origin;
    double a = 1;
    double b = 2*line.vector.dotProduct(t);
    double c = t.dotProduct(t)-m_radius*m_radius;
    double delta = b*b-4*a*c;
    if(delta<0) return NAN;
    if(delta==0)
    {
        return -b/2;
    }
    else
    {
        delta=sqrt(delta);
        double x1,x2;
        x1=(-b-delta)/2;
        x2=(-b+delta)/2;
        if(x1>0) return x1;
        return x2;
    }
}
void Sphere::broadcastObject()
{
    MPI_Bcast(this,sizeof(Sphere),MPI_BYTE,0,MPI_COMM_WORLD);
}
void Sphere::readObject(std::fstream& stream)
{
    Point3d pt;
    double rad;
    stream >> pt.x >> pt.y >> pt.z;
    stream >> rad;
    m_origin=pt;
    m_radius=rad;
    double ref,cp;
    double r,g,b;
    stream >> ref;
    stream >> cp;
    stream >> r >> g >> b;
    ncGetProperties().setColor(Vector3d(r,g,b));
    ncGetProperties().setRefractiveIndex(ref);
    ncGetProperties().setColorImpact(cp);
    std::cout << "CP:" << cp << std::endl;
    std::cout << ref << " " << r << " " << g << " " << b << std::endl;
}
