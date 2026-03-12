# include "Objects.hpp"
# include "Vec3.hpp"

class Cylinder: public Objects
{
private:
    Points3 center;
    Vec3    axis;
    double  redius;
public:
    Cylinder(Points3 _center, Vec3 _axis, double _redius);
    ~Cylinder();


    bool    intersect(const Ray& ray, double& t_hit);
};

Cylinder::Cylinder(Points3 _center, Vec3 _axis, double _redius): center(_center), axis(_axis), redius(_redius)
{
}

Cylinder::~Cylinder()
{
}


bool    Cylinder::intersect(const Ray& ray, double& t_hit) {
    // x ^2 + y^2 = r^2
    // p(t) = o + t * D
    // p(t) = ray.origine() + t * ray.diraction
    // a * t ^2 + b *t + c = 0
    // delta = 
}