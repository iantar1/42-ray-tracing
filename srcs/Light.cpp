#include "../includes/Light.hpp"
#include "../includes/utils.hpp"

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
