#pragma once
#include "Objects.hpp"
#include "Vec3.hpp"
#include "Ray.hpp"
#include "utils.hpp"
#include <cmath>

class Triangle : public Objects
{
private:
    Points3 v0, v1, v2;
    Vec3    normal;

    void computeNormal();

public:
    Triangle(const Points3& _v0, const Points3& _v1, const Points3& _v2);
    Triangle(const Points3& _v0, const Points3& _v1, const Points3& _v2, const Vec3& _color);
    Triangle(const Points3& _v0, const Points3& _v1, const Points3& _v2, const Vec3& _color, double _shininess);
    ~Triangle();

    bool intersect(const Ray& ray, double& t_hit);
    Vec3 get_normal(const Points3& hit);
    void translate(const Vec3& offset);
    void rotateX(double angle);
    void rotateY(double angle);
    void rotateZ(double angle);
};
