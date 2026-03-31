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
    ~Cylinder();

    Vec3    get_normal(const Points3& hit);


    bool    intersect(const Ray& ray, double& t_hit);
};

Cylinder::Cylinder(Points3 _center, Vec3 _axis, double _radius, double _half_height): 
    center(_center), axis(normalize(_axis)), radius(_radius), half_height(_half_height)
{
}

Cylinder::~Cylinder()
{
}


bool    Cylinder::intersect(const Ray& ray, double& t_hit) {
    // General cylinder intersection with arbitrary axis
    // Project ray onto plane perpendicular to axis, then solve quadratic

    Vec3 O = ray.getOrigin();
    Vec3 D = ray.getDirection();

    // Vector from cylinder center to ray origin
    Vec3 oc = O - this->center;

    // Project ray direction and origin onto plane perpendicular to axis
    Vec3 D_perp = D - this->axis * Vec3::dot(D, this->axis);
    Vec3 oc_perp = oc - this->axis * Vec3::dot(oc, this->axis);

    // Quadratic coefficients for intersection with infinite cylinder
    double a = Vec3::dot(D_perp, D_perp);
    if (a < 1e-8) return false;  // Ray parallel to cylinder axis

    double b = 2.0 * Vec3::dot(oc_perp, D_perp);
    double c = Vec3::dot(oc_perp, oc_perp) - radius * radius;

    // Solve quadratic equation: a*t² + b*t + c = 0
    double discriminant = b*b - 4.0*a*c;
    if (discriminant < 0.0) return false;

    double sqrt_disc = std::sqrt(discriminant);
    double t0 = (-b - sqrt_disc) / (2.0*a);
    double t1 = (-b + sqrt_disc) / (2.0*a);

    // Pick smallest positive t
    double t = (t0 > 1e-6) ? t0 : t1;
    if (t < 1e-6) return false;

    // Clamp to finite cylinder height along the axis
    Vec3 hit_point = O + D * t;
    double axis_projection = Vec3::dot(hit_point - center, this->axis);
    if (std::abs(axis_projection) > half_height) return false;

    t_hit = t;
    return true;
}

Vec3 Cylinder::get_normal(const Points3& hit)
{
    // Normal on cylinder side: the radial vector from axis to surface point
    // Formula: n = normalize(hit - center - axis * (hit - center) · axis)
    // This removes the axial component, leaving only the perpendicular (radial) component
    Vec3 oc = Vec3(
        hit.getX() - center.getX(),
        hit.getY() - center.getY(),
        hit.getZ() - center.getZ()
    );
    
    // Remove the axial component: radial = oc - axis * (oc · axis)
    Vec3 radial = oc - axis * Vec3::dot(oc, axis);
    
    return normalize(radial);
}