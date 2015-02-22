#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "geometry.h"
#include <fstream>
class LightSource
{
    public:
        LightSource();
        LightSource(Point3d pos, Vector3d light);
        virtual ~LightSource();

        const Vector3d& getLight() const;
        const Point3d& getPos() const;

        virtual void readLightSource(std::fstream& stream);
        virtual void broadcastLightSource();
    protected:
    private:
        Vector3d m_light;
        Point3d m_location;
};

#endif // LIGHTSOURCE_H
