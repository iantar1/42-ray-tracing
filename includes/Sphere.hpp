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
    Sphere(double _radius, Points3 _center, const Vec3& _color);
    Sphere(double _radius, Points3 _center, const Vec3& _color, double _shininess);
    Sphere(double _radius, Points3 _center, const Vec3& _color, double _shininess, double _transparency);
    Sphere(double _radius, Points3 _center, const Vec3& _color, double _shininess, double _transparency, double _reflectivity);
    ~Sphere();
    double getRadius() const;
    Points3 getCenter() const;
    void setRadius(double _radius);
    void setCenter(Points3 _center);
    bool intersect(const Ray& ray, double& t_hit);
    Vec3 get_normal(const Points3& hit);
    void translate(const Vec3& offset);
    void rotateX(double angle);
    void rotateY(double angle);
    void rotateZ(double angle);
};