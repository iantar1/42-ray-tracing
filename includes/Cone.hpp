# pragma once

# include "Objects.hpp"
# include "Vec3.hpp"
# include "utils.hpp"
#include <cmath>

class Cone: public Objects
{
private:
    Points3 apex;
    Vec3    axis;
    double  half_angle;
    double  half_height;  // For clamping intersections to finite Cone
public:
    Cone(Points3 _apex, Vec3 _axis, double _half_angle, double _half_height = 10.0);
    Cone(Points3 _apex, Vec3 _axis, double _half_angle, double _half_height, const Vec3& _color);
    Cone(Points3 _apex, Vec3 _axis, double _half_angle, double _half_height, const Vec3& _color, double _shininess);
    Cone(Points3 _apex, Vec3 _axis, double _half_angle, double _half_height, const Vec3& _color, double _shininess, double _transparency);
    Cone(Points3 _apex, Vec3 _axis, double _half_angle, double _half_height, const Vec3& _color, double _shininess, double _transparency, double _reflectivity);
    ~Cone();

    bool    intersect(const Ray& ray, double& t_hit);
    Vec3    get_normal(const Points3& hit);
    void translate(const Vec3& offset);
    void rotateX(double angle);
    void rotateY(double angle);
    void rotateZ(double angle);
};