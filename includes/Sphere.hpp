#pragma once
#include "Vec3.hpp"
#include "Objects.hpp"

class Sphere: public Objects
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
    bool intersect(const Ray& ray, double& t_hit);
    Vec3 get_normal(const Points3& hit);
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

void Sphere::setCenter(Points3 _center)
{
    center = _center;
}

bool Sphere::intersect(const Ray& ray, double& t_hit)
{
    // Compute the vector from ray origin to sphere center oc = ray_origin - center
    Vec3 oc = Vec3(
        ray.getOrigin().getX() - center.getX(),
        ray.getOrigin().getY() - center.getY(),
        ray.getOrigin().getZ() - center.getZ()
    );
// sphere equation: (P - C) . (P - C) = r^2
// ray equation: P = O + tD
// substitute ray into sphere: (O + tD - C) . (O + tD - C) = r^2
// expand: (O - C) . (O - C) + 2tD . (O - C) + t^2 D . D = r^2
// rearrange: t^2 D . D + 2t D . (O - C) + (O - C) . (O - C) - r^2 = 0
// This is a quadratic equation in t: at^2 + bt + c = 0
// where: a = D . D, b = 2 D . (O - C), c = (O - C) .(O - C) - r^2
// and ray_dir is D, oc is (O - C)

    Vec3 ray_dir = ray.getDirection();
    double a = Vec3::dot(ray_dir, ray_dir);
    double b = 2.0 * Vec3::dot(oc, ray_dir);
    double c = Vec3::dot(oc, oc) - radius*radius;

    double disc = b*b - 4*a*c;

    // No intersection if discriminant is negative
    // one solution: inside the sphere, two solutions: outside the sphere
    if (disc <= 0)
        return false;

    double t1 = (-b - sqrt(disc)) / (2.0 * a);
    double t2 = (-b + sqrt(disc)) / (2.0 * a);
    if (t1 > 0)
        t_hit = t1;
    else if (t2 > 0)
        t_hit = t2;
    else
        return false; // Both intersections are behind the camera
    // t_hit = (-b - sqrt(disc)) / (2.0 * a);
    return true;
}

Vec3 Sphere::get_normal(const Points3& hit)
{
    Vec3 normal = Vec3(
        hit.getX() - center.getX(),
        hit.getY() - center.getY(),
        hit.getZ() - center.getZ()
    );
    return normalize(normal);
}