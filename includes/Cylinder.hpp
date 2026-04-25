#pragma once

# include "Objects.hpp"
# include "Vec3.hpp"
# include "utils.hpp"
#include <cmath>

class Cylinder: public Objects
{
private:
    Points3 center;
    Vec3    axis;
    double  radius;
    double  half_height;  // For clamping intersections to finite cylinder
public:
    Cylinder(Points3 _center, Vec3 _axis, double _radius, double _half_height = 10.0);
    Cylinder(Points3 _center, Vec3 _axis, double _radius, double _half_height, const Vec3& _color);
    Cylinder(Points3 _center, Vec3 _axis, double _radius, double _half_height, const Vec3& _color, double _shininess);
    Cylinder(Points3 _center, Vec3 _axis, double _radius, double _half_height, const Vec3& _color, double _shininess, double _transparency);
    Cylinder(Points3 _center, Vec3 _axis, double _radius, double _half_height, const Vec3& _color, double _shininess, double _transparency, double _reflectivity);
    ~Cylinder();

    Vec3    get_normal(const Points3& hit);
    bool    intersect(const Ray& ray, double& t_hit);
    void translate(const Vec3& offset);
    void rotateX(double angle);
    void rotateY(double angle);
    void rotateZ(double angle);
};