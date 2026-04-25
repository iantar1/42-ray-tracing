# pragma once
# include "Vec3.hpp"


class Ray
{
private:
    Points3 origin;
    Vec3 direction;
public:
    Ray(Points3 _origin, Vec3 _direction);
    ~Ray();
    Points3 getOrigin() const;
    Vec3 getDirection() const;
    Points3 at(double) const;
};