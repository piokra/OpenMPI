#ifndef SCREEN_H
#define SCREEN_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "geometry.h"
class Object;
class Screen
{
public:
    Screen(int x, int y);
    virtual ~Screen();
    void gatherPixels();
    sf::Uint8* getPixels();
    void countPixels();
protected:
    sf::Color countSinglePixel(int x, int y, int samples);
    Vector3d countSingleRay(ParamLine ray, double impact,double n, int depth);

    int getOwnedRows(int rank);

    Vector3d reflectionVector(const Vector3d& normal, const Vector3d& ray);
    Vector3d refractionVector(const Vector3d& normal, const Vector3d& ray, double n1, double n2);
    double reflectance(const Vector3d& normal, const Vector3d& ray, double n1, double n2);
private:
    std::vector<Object*> m_objects;
    int m_comm_rank;
    int m_comm_size;
    sf::Uint8* m_data;
    int m_size;
    int m_yoffset;
    int m_x;
    int m_y;
    const int MAXDEPTH=16;
};

#endif // SCREEN_H
