#include "../includes/Camera.hpp"
#include "../includes/utils.hpp"
#include <cmath>

Camera::Camera()
    : position(Points3(0, 0, 0)), direction(Vec3(0, 0, 1)),
      fov(90.0), aspect_ratio((double)IMG_WIDTH / IMG_HEIGHT),
      width(IMG_WIDTH), height(IMG_HEIGHT),
      cached_fwd(0,0,1), cached_right(1,0,0), cached_up(0,1,0),
      cached_fov_scale(1.0) {}

Camera::Camera(Points3 _position, Vec3 _direction, double _fov, double _aspect_ratio)
    : position(_position), direction(_direction),
      fov(_fov), aspect_ratio(_aspect_ratio),
      width(IMG_WIDTH), height(IMG_HEIGHT),
      cached_fwd(0,0,1), cached_right(1,0,0), cached_up(0,1,0),
      cached_fov_scale(1.0) {}

Camera::~Camera() {}

Points3 Camera::getPosition() const { return position; }
Vec3    Camera::getDirection() const { return direction; }

void Camera::setPosition(const Points3 _position) { position = _position; }
void Camera::setDirection(const Vec3 _direction)  { direction = _direction; }

void Camera::setDimensions(int w, int h)
{
    width  = w;
    height = h;
    aspect_ratio = (double)w / h;
}

void Camera::updateBasis()
{
    cached_fwd = normalize(direction);

    Vec3 world_up(0, 1, 0);
    if (fabs(Vec3::dot(cached_fwd, world_up)) > 0.99)
        world_up = Vec3(1, 0, 0);

    cached_right     = normalize(Vec3::cross(world_up, cached_fwd));
    cached_up        = Vec3::cross(cached_fwd, cached_right);
    cached_fov_scale = tan(fov * M_PI / 360.0);
}

Ray Camera::getRay(int pixel_x, int pixel_y)
{
    double px = (2.0 * pixel_x / width  - 1.0) * aspect_ratio * cached_fov_scale;
    double py = (1.0 - 2.0 * pixel_y / height) * cached_fov_scale;

    Vec3 ray_dir = cached_fwd + cached_right * px + cached_up * py;
    return Ray(position, normalize(ray_dir));
}

void Camera::moveForward(double amount)
{
    Vec3 fwd = normalize(direction);
    position = position + fwd * amount;
}

void Camera::moveRight(double amount)
{
    Vec3 fwd = normalize(direction);
    Vec3 world_up(0, 1, 0);
    if (fabs(Vec3::dot(fwd, world_up)) > 0.99)
        world_up = Vec3(1, 0, 0);
    Vec3 right_v = normalize(Vec3::cross(world_up, fwd));
    position = position + right_v * amount;
}

void Camera::moveUp(double amount)
{
    position = position + Vec3(0, amount, 0);
}
