#ifndef OBJECT_H
#define OBJECT_H
#include "properties.h"
#include "geometry.h"
#include <fstream>

class Object
{
public:
    Object();
    virtual ~Object();
    virtual double getCollision(const ParamLine& line) const = 0;
    virtual Vector3d getNormal(const Point3d& point) const = 0;
    virtual void broadcastObject() = 0;
    virtual void readObject(std::fstream& stream) = 0;
    virtual const Properties& getProperties();
protected:
    virtual Properties& ncGetProperties();
private:
    Properties m_properties;
};

#endif // OBJECT_H
