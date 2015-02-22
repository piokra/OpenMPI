#include "lightsource.h"
#include <fstream>
#include "mpi.h"
LightSource::LightSource()
{
    //ctor
}

LightSource::~LightSource()
{
    //dtor
}
LightSource::LightSource(Point3d pos, Vector3d light) : m_location(pos), m_light(light)
{

}

const Vector3d& LightSource::getLight() const
{
    return m_light;
}

const Point3d& LightSource::getPos() const
{
    return m_location;
}
void LightSource::readLightSource(std::fstream& stream)
{
    stream >> m_location.x >> m_location.y >> m_location.z;
    stream >> m_light.x >> m_light.y >> m_light.z;
}

void LightSource::broadcastLightSource()
{
    MPI_Bcast(this,sizeof(LightSource),MPI_BYTE,0,MPI_COMM_WORLD);
}
