# pragma once

#define IMG_WIDTH 800
#define IMG_HEIGHT 600

# include "Vec3.hpp"
# include "Ray.hpp"
# include "utils.hpp"

class Camera
{
private:
    Points3 position;
    Vec3    direction;
    double  fov;
    double  aspect_ratio;
    int     width;
    int     height;

    Vec3   cached_fwd;
    Vec3   cached_right;
    Vec3   cached_up;
    double cached_fov_scale;

public:
    Camera();
    Camera(Points3 _position, Vec3 _direction, double _fov, double _aspect_ratio);
    Points3 getPosition() const;
    Vec3    getDirection() const;
    void    setPosition(const Points3 _position);
    void    setDirection(const Vec3 _direction);
    void    setDimensions(int w, int h);
    void    updateBasis();
    Ray     getRay(int pixel_x, int pixel_y);

    void moveForward(double amount);
    void moveRight(double amount);
    void moveUp(double amount);
    ~Camera();
};
