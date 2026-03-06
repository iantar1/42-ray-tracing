#pragma once
#include "Objects.hpp"
#include "Vec3.hpp"
#include "Ray.hpp"

class Plane: public Objects
{
private:
    Vec3 normal;
    Points3 point;
public:
    Plane(const Vec3& _normal, const Points3& _point);
    ~Plane();

    bool intersect(const Ray& ray, double& t_hit);
};

Plane::Plane(const Vec3& _normal, const Points3& _point)
    : normal(_normal), point(_point)
{
}

Plane::~Plane()
{
}


bool Plane::intersect(const Ray& ray, double& t_hit)
{
}