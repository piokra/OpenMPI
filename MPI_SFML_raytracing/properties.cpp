#include "properties.h"

Properties::Properties()
{
    //ctor
}

Properties::~Properties()
{
    //dtor
}
const Vector3d& Properties::getColor() const
{
    return m_color;
}

const double& Properties::getRefractiveIndex() const
{
    return m_refractive_index;
}

void Properties::setRefractiveIndex(double index)
{
    m_refractive_index=index;
}

void Properties::setColor(Vector3d color)
{
    m_color=color;
}
const double& Properties::getColorImpact() const
{
    return m_color_impact;
}

void Properties::setColorImpact(double coeff)
{
    m_color_impact=coeff;
}
