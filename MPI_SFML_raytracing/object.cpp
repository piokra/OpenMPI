#include "object.h"

Object::Object()
{
    //ctor
}

Object::~Object()
{
    //dtor
}

const Properties& Object::getProperties()
{
    return m_properties;
}
Properties& Object::ncGetProperties()
{
    return m_properties;
}
