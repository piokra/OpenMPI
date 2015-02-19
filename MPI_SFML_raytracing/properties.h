#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <SFML/Graphics.hpp>
#include "geometry.h"
class Properties
{
public:
    Properties();
    virtual ~Properties();

    const Vector3d& getColor() const;
    const double& getRefractiveIndex() const;
    const double& getColorImpact() const;

    void setRefractiveIndex(double index);
    void setColorImpact(double coeff);
    void setColor(Vector3d color);
protected:
private:
    double m_refractive_index;
    double m_color_impact;

    Vector3d m_color;
};

#endif // PROPERTIES_H
