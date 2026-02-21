# pragma once

#define IMG_WIDTH 800
#define IMG_HEIGHT 600

# include "Vec3.hpp"
# include "Ray.hpp"
# include "../srcs/utils.cpp"

class Camera
{
private:
    Points3 position; // x, y, z coordinates
    Vec3 direction; // direction vector
    Vec3 forward;
    Vec3 right;
    Vec3 up;
    double fov;
    double aspect_ratio;

public:
    Camera();
    Camera(Points3 _position, Vec3 _direction, double _fov, double _aspect_ratio);
    Points3 getPosition() const;
    Vec3 getDirection() const;
    void setPosition(const Points3 _position);
    void setDirection(const Vec3 _direction);
    Ray getRay(int pixel_x, int pixel_y);
    
    // Movement
    void moveForward(double amount);
    void moveRight(double amount);
    void moveUp(double amount);
    ~Camera();
};

Camera::Camera(){}

Camera::Camera(Points3 _position, Vec3 _direction, double _fov, double _aspect_ratio) : position(_position), direction(_direction), fov(_fov), aspect_ratio(_aspect_ratio) {}

Camera::~Camera(){}


Points3 Camera::getPosition() const
{
    return position;
}

Vec3 Camera::getDirection() const
{
    return direction;
}

void Camera::setPosition(const Points3 _position)
{
    position = _position;
}

void Camera::setDirection(const Vec3 _direction)
{
    direction = _direction;
}

Ray Camera::getRay(int pixel_x, int pixel_y)
{
    double px = (2.0 * pixel_x / IMG_WIDTH) - 1.0;
    double py = 1.0 - (2.0 * pixel_y / IMG_HEIGHT);

    Vec3 ray_dir(px, py, -1);
    return Ray(position, ray_dir);
}

void Camera::moveForward(double amount)
{
    Vec3 forward = normalize(this->direction);
    this->position = this->position + forward * amount;
}

void Camera::moveRight(double amount)
{
     // right vector
    
    Vec3 right = normalize(Vec3::cross(this->direction, Vec3(0, 1, 0)));// we normalize:  we direction only, not the length.
    this->position = this->position + right * amount;
}

void Camera::moveUp(double amount)
{
    Vec3 up(0,1,0);
    this->position = this->position + up * amount;
}
