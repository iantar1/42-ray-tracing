#pragma once
#include "Vec3.hpp"

# include "../includes/Camera.hpp"
# include "../includes/Ray.hpp"
# include "../includes/Vec3.hpp"
#include <math.h>

static inline Vec3 normalize(Vec3 v)
{
    double len = sqrt(Vec3::dot(v, v));
    return Vec3(v.getX()/len, v.getY()/len, v.getZ()/len);
}

bool intersect_sphere(
    const Vec3& ray_origin,
    const Vec3& ray_dir,
    const Vec3& center,
    double radius,
    double& t_hit
)
{
    // Compute the vector from ray origin to sphere center oc = ray_origin - center
    Vec3 oc = Vec3(
        ray_origin.getX() - center.getX(),
        ray_origin.getY() - center.getY(),
        ray_origin.getZ() - center.getZ()
    );
// sphere equation: (P - C) . (P - C) = r^2
// ray equation: P = O + tD
// substitute ray into sphere: (O + tD - C) . (O + tD - C) = r^2
// expand: (O - C) . (O - C) + 2tD . (O - C) + t^2 D . D = r^2
// rearrange: t^2 D . D + 2t D . (O - C) + (O - C) . (O - C) - r^2 = 0
// This is a quadratic equation in t: at^2 + bt + c = 0
// where: a = D . D, b = 2 D . (O - C), c = (O - C) .(O - C) - r^2
// and ray_dir is D, oc is (O - C)

    double a = Vec3::dot(ray_dir, ray_dir);
    double b = 2.0 * Vec3::dot(oc, ray_dir);
    double c = Vec3::dot(oc, oc) - radius*radius;

    double disc = b*b - 4*a*c;

    // No intersection if discriminant is negative
    // one solution: inside the sphere, two solutions: outside the sphere
    if (disc <= 0)
        return false;

    double t1 = (-b - sqrt(disc)) / (2.0 * a);
    double t2 = (-b + sqrt(disc)) / (2.0 * a);
    if (t1 > 0)
        t_hit = t1;
    else if (t2 > 0)
        t_hit = t2;
    else
        return false; // Both intersections are behind the camera
    // t_hit = (-b - sqrt(disc)) / (2.0 * a);
    return true;
}

int compute_lighting(const Vec3& hit, const Vec3& center)
{
    Vec3 normal = Vec3(
        hit.getX() - center.getX(),
        hit.getY() - center.getY(),
        hit.getZ() - center.getZ()
    );
    normal = normalize(normal);

    Vec3 light_dir = normalize(Vec3(-1, 1, -1));// Vec3(-1, 1, -1) is the direction from the hit point to the light source

    double brightness = Vec3::dot(normal, light_dir);
    if (brightness < 0)
        brightness = 0;

    int c = (int)(brightness * 255.0);
    return (c << 16) | (c << 8) | c;// Return grayscale color based on brightness (move the c value to the red, green, and blue channels)
}

static inline void write_pixel(void* data, int offset, int color)
{
    *(unsigned int*)((char*)data + offset) = color;
}