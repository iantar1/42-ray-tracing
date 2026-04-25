# pragma once

#define IMG_WIDTH 800
#define IMG_HEIGHT 600

# include "Vec3.hpp"
# include "Ray.hpp"
# include "utils.hpp"

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
