#include "../includes/Camera.hpp"
#include "../includes/utils.hpp"

Camera::Camera(){}

Camera::Camera(Points3 _position, Vec3 _direction, double _fov, double _aspect_ratio) 
    : position(_position), direction(_direction), fov(_fov), aspect_ratio(_aspect_ratio) {}

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
    double px = (2.0 * pixel_x / IMG_WIDTH) - 1.0;// we want to map pixel_x from [0, IMG_WIDTH] to [-1, 1]
    double py = 1.0 - (2.0 * pixel_y / IMG_HEIGHT);// we want to map pixel_y from [0, IMG_HEIGHT] to [1, -1]

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
