# include "Objects.hpp"
# include "Vec3.hpp"

class Cylinder: public Objects
{
private:
    Points3 center;
    Vec3    axis;
    double  radius;
public:
    Cylinder(Points3 _center, Vec3 _axis, double _redius);
    ~Cylinder();
};

Cylinder::Cylinder(Points3 _center, Vec3 _axis, double _redius): center(_center), axis(_axis), redius(_redius)
{
}

Cylinder::~Cylinder()
{
}
