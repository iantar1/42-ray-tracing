#include "../includes/Square.hpp"
#include <cmath>

void Square::computeTangents()
{
    Vec3 nrm = normalize(normal);
    normal = nrm;
    Vec3 world_up(0, 1, 0);
    if (fabs(Vec3::dot(nrm, world_up)) > 0.99)
        world_up = Vec3(1, 0, 0);
    u = normalize(Vec3::cross(world_up, nrm));
    v = Vec3::cross(nrm, u);
}

Square::Square(const Points3& _center, const Vec3& _normal, double _half_size)
    : Objects(), center(_center), normal(_normal), half_size(_half_size)
{
    computeTangents();
}

Square::Square(const Points3& _center, const Vec3& _normal, double _half_size, const Vec3& _color)
    : Objects(_color), center(_center), normal(_normal), half_size(_half_size)
{
    computeTangents();
}

Square::Square(const Points3& _center, const Vec3& _normal, double _half_size, const Vec3& _color, double _shininess)
    : Objects(_color, _shininess), center(_center), normal(_normal), half_size(_half_size)
{
    computeTangents();
}

Square::~Square() {}

bool Square::intersect(const Ray& ray, double& t_hit)
{
    double denom = Vec3::dot(normal, ray.getDirection());
    if (fabs(denom) < 1e-8)
        return false;

    Vec3 oc = center - ray.getOrigin();
    double t = Vec3::dot(oc, normal) / denom;
    if (t < 1e-4)
        return false;

    Points3 hit = ray.at(t);
    Vec3 d = hit - center;

    if (fabs(Vec3::dot(d, u)) <= half_size && fabs(Vec3::dot(d, v)) <= half_size)
    {
        t_hit = t;
        return true;
    }
    return false;
}

Vec3 Square::get_normal(const Points3& hit)
{
    (void)hit;
    return normal;
}

void Square::translate(const Vec3& offset)
{
    center = center + offset;
}

void Square::rotateX(double angle)
{
    center = rotateAroundAxis(center, Vec3(1, 0, 0), angle);
    normal = rotateAroundAxis(normal, Vec3(1, 0, 0), angle);
    computeTangents();
}

void Square::rotateY(double angle)
{
    center = rotateAroundAxis(center, Vec3(0, 1, 0), angle);
    normal = rotateAroundAxis(normal, Vec3(0, 1, 0), angle);
    computeTangents();
}

void Square::rotateZ(double angle)
{
    center = rotateAroundAxis(center, Vec3(0, 0, 1), angle);
    normal = rotateAroundAxis(normal, Vec3(0, 0, 1), angle);
    computeTangents();
}
