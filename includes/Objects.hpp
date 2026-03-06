#pragma once
#include "Ray.hpp"

class Objects
{
private:

public:
    Objects(/* args */);
    ~Objects();

    virtual bool intersect(const Ray& ray, double& t_hit) = 0;
    virtual Vec3 get_normal(const Points3& hit) = 0;

};

Objects::Objects(/* args */)
{
}

Objects::~Objects()
{
}
