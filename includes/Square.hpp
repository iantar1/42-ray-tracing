#pragma once
#include "Objects.hpp"
#include "Vec3.hpp"
#include "Ray.hpp"
#include "utils.hpp"
#include <cmath>

class Square : public Objects
{
private:
    Points3 center;
    Vec3    normal;
    double  half_size;
    Vec3    u, v;

    void computeTangents();

public:
    Square(const Points3& _center, const Vec3& _normal, double _half_size);
    Square(const Points3& _center, const Vec3& _normal, double _half_size, const Vec3& _color);
    Square(const Points3& _center, const Vec3& _normal, double _half_size, const Vec3& _color, double _shininess);
    ~Square();

    bool intersect(const Ray& ray, double& t_hit);
    Vec3 get_normal(const Points3& hit);
    void translate(const Vec3& offset);
    void rotateX(double angle);
    void rotateY(double angle);
    void rotateZ(double angle);
};
