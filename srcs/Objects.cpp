#include "../includes/Objects.hpp"

Objects::Objects(): color(Vec3(1.0, 1.0, 1.0)), shininess(32.0), transparency(0.0), reflectivity(0.0)
{
}

Objects::Objects(const Vec3& _color): color(_color), shininess(32.0), transparency(0.0), reflectivity(0.0)
{
}

Objects::Objects(const Vec3& _color, double _shininess): color(_color), shininess(_shininess), transparency(0.0), reflectivity(0.0)
{
}

Objects::Objects(const Vec3& _color, double _shininess, double _transparency): color(_color), shininess(_shininess), transparency(_transparency), reflectivity(0.0)
{
}

Objects::Objects(const Vec3& _color, double _shininess, double _transparency, double _reflectivity): color(_color), shininess(_shininess), transparency(_transparency), reflectivity(_reflectivity)
{
}

Objects::~Objects()
{
}

Vec3 Objects::getColor() const
{
    return color;
}

void Objects::setColor(const Vec3& _color)
{
    color = _color;
}

double Objects::getShininess() const
{
    return shininess;
}

void Objects::setShininess(double _shininess)
{
    shininess = _shininess;
}

double Objects::getTransparency() const
{
    return transparency;
}

void Objects::setTransparency(double _transparency)
{
    transparency = _transparency;
}

double Objects::getReflectivity() const
{
    return reflectivity;
}

void Objects::setReflectivity(double _reflectivity)
{
    reflectivity = _reflectivity;
}
