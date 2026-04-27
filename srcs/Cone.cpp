#include "../includes/Cone.hpp"
#include "../includes/utils.hpp"
#include <cmath>

Cone::Cone(Points3 _apex, Vec3 _axis, double _half_angle, double _half_height):
    Objects(), apex(_apex), axis(normalize(_axis)), half_angle(_half_angle), half_height(_half_height)
{
}

Cone::Cone(Points3 _apex, Vec3 _axis, double _half_angle, double _half_height, const Vec3& _color):
    Objects(_color), apex(_apex), axis(normalize(_axis)), half_angle(_half_angle), half_height(_half_height)
{
}

Cone::Cone(Points3 _apex, Vec3 _axis, double _half_angle, double _half_height, const Vec3& _color, double _shininess):
    Objects(_color, _shininess), apex(_apex), axis(normalize(_axis)), half_angle(_half_angle), half_height(_half_height)
{
}

Cone::Cone(Points3 _apex, Vec3 _axis, double _half_angle, double _half_height, const Vec3& _color, double _shininess, double _transparency):
    Objects(_color, _shininess, _transparency), apex(_apex), axis(normalize(_axis)), half_angle(_half_angle), half_height(_half_height)
{
}

Cone::Cone(Points3 _apex, Vec3 _axis, double _half_angle, double _half_height, const Vec3& _color, double _shininess, double _transparency, double _reflectivity):
    Objects(_color, _shininess, _transparency, _reflectivity), apex(_apex), axis(normalize(_axis)), half_angle(_half_angle), half_height(_half_height)
{
}

Cone::~Cone()
{
}

bool Cone::intersect(const Ray& ray, double& t_hit)
{
    // Ray: R(t) = O + t*D
    // Cone defined by: apex, axis (unit direction), half_angle θ
    // Cone surface: (P - apex) · axis = cos(θ) * |P - apex|
    // We can multiply the second equation by itself to work with it, then reorder things a bit.\
    // ((P - apex) .axis).((P - apex) .axis) = (cos(θ) * |P - apex|) * (cos(θ) * |P - apex|)
    // ((P - apex) .axis) ^ 2 = cos(θ)^2 * (P - apex)^2
    // replace R(t) with P
    // ((O + t*D - apex) · axis)^2 = cos^2(θ) * |O + t*D - apex|^2
    // Let oc = O - apex
    // (oc·axis + t(D·axis))^2 = cos^2(θ) * (oc + t*D)·(oc + t*D)
    // (oc·axis)^2 + 2t(oc·axis)(D·axis) + t^2(D·axis)^2 = cos^2(θ)[oc·oc + 2t(oc·D) + t^2(D·D)]
    // t^2[(D·axis)^2 - cos^2(θ)(D·D)] + t[2(oc·axis)(D·axis) - 2cos^2(θ)(oc·D)] + [(oc·axis)^2 - cos^2(θ)(oc·oc)] = 0
    // 
    
    Points3 O = ray.getOrigin();
    Vec3 D = ray.getDirection();
    
    // oc = O - apex
    Vec3 oc = Vec3(
        O.getX() - apex.getX(),
        O.getY() - apex.getY(),
        O.getZ() - apex.getZ()
    );
    
    // Precompute some dot products
    double cos_theta = cos(half_angle);
    double cos_sq = cos_theta * cos_theta;
    
    double D_dot_axis = Vec3::dot(D, axis);
    double oc_dot_axis = Vec3::dot(oc, axis);
    double D_dot_D = Vec3::dot(D, D);
    double oc_dot_D = Vec3::dot(oc, D);
    double oc_dot_oc = Vec3::dot(oc, oc);
    
    // Quadratic equation coefficients: at^2 + bt + c = 0
    // a = (D·axis)^2 - cos^2(θ) * (D·D)
    double a = D_dot_axis * D_dot_axis - cos_sq * D_dot_D;
    
    // b = 2[(D·axis)(oc·axis) - cos^2(θ)(oc·D)]
    double b = 2.0 * (D_dot_axis * oc_dot_axis - cos_sq * oc_dot_D);
    
    // c = (oc·axis)^2 - cos^2(θ)(oc·oc)
    double c = oc_dot_axis * oc_dot_axis - cos_sq * oc_dot_oc;
    
    // Check discriminant
    double disc = b * b - 4.0 * a * c;
    double t_cone = -1.0;
    
    if (disc >= 0.0)
    {
        double sqrt_disc = sqrt(disc);
        
        // Solve quadratic: t = (-b ± sqrt(discriminant)) / (2a)
        double t1 = (-b - sqrt_disc) / (2.0 * a);
        double t2 = (-b + sqrt_disc) / (2.0 * a);
        
        // Try the smallest positive t first
        double t = -1.0;
        if (t1 > 0.0)
            t = t1;
        else if (t2 > 0.0)
            t = t2;
        
        if (t > 0.0)
        {
            // Validate the hit by checking height constraints
            // (hit - apex) · axis should be between 0 and half_height
            Points3 hit = ray.at(t);
            Vec3 hit_to_apex = Vec3(
                hit.getX() - apex.getX(),
                hit.getY() - apex.getY(),
                hit.getZ() - apex.getZ()
            );
            
            double height_projection = Vec3::dot(hit_to_apex, axis);
            
            // Accept if within the cone height
            if (height_projection >= 0.0 && height_projection <= half_height)
                t_cone = t;
        }
    }
    
    // Check intersection with the base (circular disk at height = half_height)
    double t_base = -1.0;
    if (fabs(D_dot_axis) > 1e-6) // Not parallel to axis
    {
        // Plane equation: (P - apex) · axis = half_height
        // Solve: (O + t*D - apex) · axis = half_height
        // (oc + t*D) · axis = half_height
        // oc · axis + t * (D · axis) = half_height
        double t = (half_height - oc_dot_axis) / D_dot_axis;
        
        if (t > 0.0)
        {
            Points3 hit = ray.at(t);
            Vec3 hit_to_apex = Vec3(
                hit.getX() - apex.getX(),
                hit.getY() - apex.getY(),
                hit.getZ() - apex.getZ()
            );
            
            // Check if hit point is within the cone base radius at height = half_height
            // The base radius is the radius at the bottom of the cone
            double base_radius = half_height * tan(half_angle);
            
            Vec3 axis_proj = axis * Vec3::dot(hit_to_apex, axis);
            Vec3 perp = hit_to_apex - axis_proj;
            if (Vec3::dot(perp, perp) <= base_radius * base_radius)
                t_base = t;
        }
    }
    
    // Return the closest valid intersection
    if (t_cone > 0.0 && (t_base < 0.0 || t_cone < t_base))
    {
        t_hit = t_cone;
        return true;
    }
    else if (t_base > 0.0)
    {
        t_hit = t_base;
        return true;
    }
    
    return false;
}

Vec3 Cone::get_normal(const Points3& hit)
{
    // Calculate the normal at the hit point
    Vec3 hit_minus_apex = Vec3(
        hit.getX() - apex.getX(),
        hit.getY() - apex.getY(),
        hit.getZ() - apex.getZ()
    );
    
    // Component along axis
    double projection = Vec3::dot(hit_minus_apex, axis);
    
    // Check if we're on the base (height ≈ half_height)
    if (fabs(projection - half_height) < 1e-6)
    {
        // Normal points along the positive axis (outward from base)
        return axis;
    }
    
    // On the conical surface
    // Perpendicular component from axis
    Vec3 perp = hit_minus_apex - (axis * projection);
    
    // Normal for cone surface (perpendicular radial component)
    return normalize(perp);
}

void Cone::translate(const Vec3& offset)
{
    apex = Points3(
        apex.getX() + offset.getX(),
        apex.getY() + offset.getY(),
        apex.getZ() + offset.getZ()
    );
}

void Cone::rotateX(double angle)
{
    // Rotate axis around X-axis
    double y = axis.getY();
    double z = axis.getZ();
    double cos_a = cos(angle);
    double sin_a = sin(angle);
    axis = normalize(Vec3(axis.getX(), y * cos_a - z * sin_a, y * sin_a + z * cos_a));
}

void Cone::rotateY(double angle)
{
    // Rotate axis around Y-axis
    double x = axis.getX();
    double z = axis.getZ();
    double cos_a = cos(angle);
    double sin_a = sin(angle);
    axis = normalize(Vec3(x * cos_a + z * sin_a, axis.getY(), -x * sin_a + z * cos_a));
}

void Cone::rotateZ(double angle)
{
    // Rotate axis around Z-axis
    double x = axis.getX();
    double y = axis.getY();
    double cos_a = cos(angle);
    double sin_a = sin(angle);
    axis = normalize(Vec3(x * cos_a - y * sin_a, x * sin_a + y * cos_a, axis.getZ()));
}
