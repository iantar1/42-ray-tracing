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
    Plane(const Vec3& _normal, const Points3& _point, const Vec3& _color);
    Plane(const Vec3& _normal, const Points3& _point, const Vec3& _color, double _shininess);
    Plane(const Vec3& _normal, const Points3& _point, const Vec3& _color, double _shininess, double _transparency);
    Plane(const Vec3& _normal, const Points3& _point, const Vec3& _color, double _shininess, double _transparency, double _reflectivity);
    ~Plane();

    bool    intersect(const Ray& ray, double& t_hit);
    Vec3    get_normal();
    Vec3    get_normal(const Points3& hit);
    void translate(const Vec3& offset);
    void rotateX(double angle);
    void rotateY(double angle);
    void rotateZ(double angle);
};