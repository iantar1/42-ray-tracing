#include "../includes/Plane.hpp"
#include "../includes/utils.hpp"
#include <cmath>

Plane::Plane(const Vec3& _normal, const Points3& _point)
    : Objects(), normal(_normal), point(_point)
{
}

Plane::Plane(const Vec3& _normal, const Points3& _point, const Vec3& _color)
    : Objects(_color), normal(_normal), point(_point)
{
}

Plane::Plane(const Vec3& _normal, const Points3& _point, const Vec3& _color, double _shininess)
    : Objects(_color, _shininess), normal(_normal), point(_point)
{
}

Plane::Plane(const Vec3& _normal, const Points3& _point, const Vec3& _color, double _shininess, double _transparency)
    : Objects(_color, _shininess, _transparency), normal(_normal), point(_point)
{
}

Plane::Plane(const Vec3& _normal, const Points3& _point, const Vec3& _color, double _shininess, double _transparency, double _reflectivity)
    : Objects(_color, _shininess, _transparency, _reflectivity), normal(_normal), point(_point)
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

Vec3 Plane::get_normal(const Points3& hit) {
    return normalize(this->normal);
}

Vec3 Plane::get_normal() {
    return normalize(this->normal);
}

void Plane::translate(const Vec3& offset)
{
    point = Points3(
        point.getX() + offset.getX(),
        point.getY() + offset.getY(),
        point.getZ() + offset.getZ()
    );
}

void Plane::rotateX(double angle)
{
    // Rotate normal around X-axis
    double y = normal.getY();
    double z = normal.getZ();
    double cos_a = cos(angle);
    double sin_a = sin(angle);
    normal = Vec3(normal.getX(), y * cos_a - z * sin_a, y * sin_a + z * cos_a);
}

void Plane::rotateY(double angle)
{
    // Rotate normal around Y-axis
    double x = normal.getX();
    double z = normal.getZ();
    double cos_a = cos(angle);
    double sin_a = sin(angle);
    normal = Vec3(x * cos_a + z * sin_a, normal.getY(), -x * sin_a + z * cos_a);
}

void Plane::rotateZ(double angle)
{
    // Rotate normal around Z-axis
    double x = normal.getX();
    double y = normal.getY();
    double cos_a = cos(angle);
    double sin_a = sin(angle);
    normal = Vec3(x * cos_a - y * sin_a, x * sin_a + y * cos_a, normal.getZ());
}
