#include "../includes/Triangle.hpp"
#include <cmath>

void Triangle::computeNormal()
{
    Vec3 e1 = v1 - v0;
    Vec3 e2 = v2 - v0;
    normal = normalize(Vec3::cross(e1, e2));
}

Triangle::Triangle(const Points3& _v0, const Points3& _v1, const Points3& _v2)
    : Objects(), v0(_v0), v1(_v1), v2(_v2)
{
    computeNormal();
}

Triangle::Triangle(const Points3& _v0, const Points3& _v1, const Points3& _v2, const Vec3& _color)
    : Objects(_color), v0(_v0), v1(_v1), v2(_v2)
{
    computeNormal();
}

Triangle::Triangle(const Points3& _v0, const Points3& _v1, const Points3& _v2, const Vec3& _color, double _shininess)
    : Objects(_color, _shininess), v0(_v0), v1(_v1), v2(_v2)
{
    computeNormal();
}

Triangle::~Triangle() {}

// Möller–Trumbore intersection
bool Triangle::intersect(const Ray& ray, double& t_hit)
{
    Vec3 e1 = v1 - v0;
    Vec3 e2 = v2 - v0;
    Vec3 dir = ray.getDirection();
    Vec3 h = Vec3::cross(dir, e2);
    double a = Vec3::dot(e1, h);

    if (fabs(a) < 1e-8)
        return false;

    double f = 1.0 / a;
    Vec3 s = ray.getOrigin() - v0;
    double u = f * Vec3::dot(s, h);

    if (u < 0.0 || u > 1.0)
        return false;

    Vec3 q = Vec3::cross(s, e1);
    double v = f * Vec3::dot(dir, q);

    if (v < 0.0 || u + v > 1.0)
        return false;

    double t = f * Vec3::dot(e2, q);
    if (t < 1e-4)
        return false;

    t_hit = t;
    return true;
}

Vec3 Triangle::get_normal(const Points3& hit)
{
    (void)hit;
    return normal;
}

void Triangle::translate(const Vec3& offset)
{
    v0 = v0 + offset;
    v1 = v1 + offset;
    v2 = v2 + offset;
}

void Triangle::rotateX(double angle)
{
    v0 = rotateAroundAxis(v0, Vec3(1, 0, 0), angle);
    v1 = rotateAroundAxis(v1, Vec3(1, 0, 0), angle);
    v2 = rotateAroundAxis(v2, Vec3(1, 0, 0), angle);
    computeNormal();
}

void Triangle::rotateY(double angle)
{
    v0 = rotateAroundAxis(v0, Vec3(0, 1, 0), angle);
    v1 = rotateAroundAxis(v1, Vec3(0, 1, 0), angle);
    v2 = rotateAroundAxis(v2, Vec3(0, 1, 0), angle);
    computeNormal();
}

void Triangle::rotateZ(double angle)
{
    v0 = rotateAroundAxis(v0, Vec3(0, 0, 1), angle);
    v1 = rotateAroundAxis(v1, Vec3(0, 0, 1), angle);
    v2 = rotateAroundAxis(v2, Vec3(0, 0, 1), angle);
    computeNormal();
}
