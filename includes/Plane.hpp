#pragma once
#include "Objects.hpp"
#include "Vec3.hpp"
#include "Ray.hpp"
# include "utils.hpp"

class Plane: public Objects
{
private:
    Vec3 normal;
    Points3 point;
public:
    Plane(const Vec3& _normal, const Points3& _point);
    ~Plane();

    bool    intersect(const Ray& ray, double& t_hit);
    Vec3    get_normal();
    Vec3    get_normal(const Points3& hit);
};

Plane::Plane(const Vec3& _normal, const Points3& _point)
    : normal(_normal), point(_point)
{
}

Plane::~Plane()
{
}

// ( p - p0 ) * normal = 0 (or cross(p - p0, normal) = 0)
// p and p0 are points in the plan
// ray: p = origne + t * dir
// merge: (ray_origine + t * ray_dir - p0) * normal = 0
//  ray_origine * normal + t * normal * ray_dir - po * normal = 0
// t = ((p0 - ray_origine) * normal ) / (ray_dir * normal)
bool Plane::intersect(const Ray& ray, double& t_hit)
{
    // vector from ray source to plan
    double denom = Vec3::dot(ray.getDirection(), this->normal);
    if (std::abs(denom) < 1e-8)
    {
        //ray is parallel to the plan
        return false;
    }
    t_hit = (Vec3::dot(this->point - ray.getOrigin(),  this->normal)) / denom;
    return t_hit >= 0;
}

Vec3    Plane::get_normal(const Points3& hit) {
    return normalize(this->normal);
}