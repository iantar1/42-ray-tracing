#include "../includes/Ray.hpp"

Ray::Ray(Points3 _origin, Vec3 _direction) : origin(_origin), direction(_direction)
{
}

Ray::~Ray()
{
}

Points3 Ray::getOrigin() const
{
    return origin;
}

Vec3 Ray::getDirection() const
{
    return direction;
}

Points3 Ray::at(double t) const
{
    return Points3(
        origin.getX() + t*direction.getX(),
        origin.getY() + t*direction.getY(),
        origin.getZ() + t*direction.getZ()
    );
}
