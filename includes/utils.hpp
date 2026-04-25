#pragma once
#include "Vec3.hpp"

# include "../includes/Camera.hpp"
# include "../includes/Ray.hpp"
# include "../includes/Vec3.hpp"
#include <math.h>

Vec3 normalize(Vec3 v);

bool intersect_sphere(
    const Vec3& ray_origin,
    const Vec3& ray_dir,
    const Vec3& center,
    double radius,
    double& t_hit
);

void write_pixel(void* data, int offset, int color);

Vec3 pixel_to_ray(int x, int y);

Vec3 rotateAroundAxis(Vec3 v, Vec3 axis, double theta);
