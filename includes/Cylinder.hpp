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
    // Cylinder equation: x² + z² = r² (aligned along y-axis)
    // Ray equation: P(t) = O + t*D = (Ox+tDx, Oy+tDy, Oz+tDz)
    // Substitute into cylinder equation (ignoring y):
    // (Ox+tDx)² + (Oz+tDz)² = r²
    // Expand: (Dx²+Dz²)t² + 2(Ox*Dx+Oz*Dz)t + (Ox²+Oz²-r²) = 0
    // This is quadratic: at² + bt + c = 0
    
    Vec3 O = ray.getOrigin();
    Vec3 D = ray.getDirection();
    
    double Ox = O.getX() - center.getX();
    double Oz = O.getZ() - center.getZ();
    double Dx = D.getX();
    double Dz = D.getZ();
    
    // Quadratic coefficients
    double a = Dx*Dx + Dz*Dz;
    if (a < 1e-8) return false;  // Ray parallel to y-axis
    
    double b = 2.0 * (Ox*Dx + Oz*Dz);
    double c = Ox*Ox + Oz*Oz - radius*radius;
    
    // Solve quadratic
    double discriminant = b*b - 4.0*a*c;
    if (discriminant < 0.0) return false;
    
    double sqrt_disc = std::sqrt(discriminant);
    double t0 = (-b - sqrt_disc) / (2.0*a);
    double t1 = (-b + sqrt_disc) / (2.0*a);
    
    // Pick smallest positive t
    double t = (t0 > 1e-6) ? t0 : t1;
    if (t < 1e-6) return false;
    
    // Clamp to finite cylinder height (along y-axis)
    Vec3 hit_point = O + D * t;
    double y_projection = hit_point.getY() - center.getY();
    if (std::abs(y_projection) > half_height) return false;
    
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