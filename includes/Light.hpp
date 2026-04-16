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

Light::Light(): intensity(1), angle(0), position(Vec3(-1, 1, -1))
{
}

Light::Light(double _intensity, double _angle, Vec3 _position) : intensity(_intensity), angle(_angle), position(_position)
{
}

Light::~Light()
{
}

void Light::moveUpDown(double amount) {
    // this->angle += amount;
    this->position = rotateAroundAxis(this->position, Vec3(1, 0, 0), amount);
}
void Light::moveRightLeft(double amount) {
    // amount += amount;
    this->position = rotateAroundAxis(this->position, Vec3(0, 1, 0), amount);
}

Vec3 Light::getPosition() const {
    return this->position;
}

double Light::getIntensity() const {
    return this->intensity;
}