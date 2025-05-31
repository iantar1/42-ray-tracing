# pragma once

# include "Vec3.hpp"

class Camera
{
private:
    Points3 position; // x, y, z coordinates
    Vec3 direction; // direction vector
    double fov = 90.0;          // field of view
    double aspect_ratio = 16.0 / 9.0; // aspect ratio of the camera
public:
    Camera();
    ~Camera();
};

Camera::Camera(){}

Camera::~Camera(){}


