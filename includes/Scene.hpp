#pragma once
#include <vector>
#include "Sphere.hpp"
#include "Camera.hpp"
# include "includes/utils.hpp"

class Scene
{
private:
    // it must contain class of types objects, and other objects will inherit from it.
    std::vector<Sphere> objects;
    Vec3 light_dir;
    int size_line;
    int bytes_per_pixel;
    void* image_data;
public:
    int computeLighting(const Vec3& hit, const Sphere& obj);
    void render(Camera& camera);
    Scene(/* args */);
    ~Scene();
};

Scene::Scene(/* args */)
{
}

Scene::~Scene()
{
}


void Scene::render(Camera& camera)
{
    int bytes = bytes_per_pixel / 8;
    // std::cout << "Rendering scene with camera at (" << camera.getPosition().getX() << ", " << camera.getPosition().getY() << ", " << camera.getPosition().getZ() << ")\n";
    Points3 sphere_center(0, 0, -3);
    double radius = 1.0;

    for (int y = 0; y < IMG_HEIGHT; y++)
    {
        for (int x = 0; x < IMG_WIDTH; x++)
        {
            int offset = y * size_line + x * bytes;

            Ray ray = camera.getRay(x, y);

            double t;
            if (intersect_sphere(ray.getOrigin(), ray.getDirection(), sphere_center, radius, t))
            {
                Points3 hit = ray.at(t);
                int color = compute_lighting(hit, sphere_center);
                write_pixel(image_data, offset, color);
            }
            else
            {
                write_pixel(image_data, offset, 0x000000FF);
            }
        }
    }
}



