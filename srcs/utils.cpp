# include "../includes/Camera.hpp"
# include "../includes/Ray.hpp"
# include "../includes/Vec3.hpp"
#include <math.h>

static inline Vec3 normalize(Vec3 v)
{
    double len = sqrt(Vec3::dot(v, v));
    return Vec3(v.getX()/len, v.getY()/len, v.getZ()/len);
}
