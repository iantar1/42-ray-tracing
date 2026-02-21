#pragma once
#include "Vec3.hpp"

class Sphere
{
private:
    double radius;
    Points3 center;
public:
    Sphere(double _radius, Points3 _center);
    ~Sphere();
    double getRadius() const;
    Points3 getCenter() const;
    void setRadius(double _radius);
    void setCenter(Points3 _center);
};

Sphere::Sphere(double _radius, Points3 _center) : radius(_radius), center(_center)
{
}

Sphere::~Sphere()
{
}

double Sphere::getRadius() const
{
    return radius;
}

Vec3 Sphere::getCenter() const
{
    return center;
}

void Sphere::setRadius(double _radius)
{
    radius = _radius;
}

void Sphere::setCenter(Vec3 _center)
{
    center = _center;
}