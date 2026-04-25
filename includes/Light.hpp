#pragma once

#include "Vec3.hpp"
#include "utils.hpp"

class Light
{
private:
    double intensity;
    double angle;
    Vec3 position;
public:
    Light(/* args */);
    Light(double _intensity, double _angle, Vec3 _position);
    ~Light();

    void moveUpDown(double amount);
    void moveRightLeft(double amount);
    
    Vec3 getPosition() const;
    double getIntensity() const;
};