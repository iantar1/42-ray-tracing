#pragma once
#include <vector>
#include "Sphere.hpp"
#include "Camera.hpp"
#include "utils.hpp"
#include <algorithm>

class Scene
{
private:
    // it must contain class of types objects, and other objects will inherit from it.
    std::vector<Sphere> objects;
    Points3 scene_center;
    Points3 light_pos;
    double light_angle;
    int size_line;
    int bytes_per_pixel;
    void* image_data;
public:

    Scene();
    Scene(int _size_line, int _bytes_per_pixel, void* _image_data);
    ~Scene();
    

    int computeLighting(const Vec3& hit, const Sphere& obj);
    void render(Camera& camera);
    void addObject(const Sphere& obj);
    int compute_lighting(const Vec3& hit, const Vec3& center);
    void setLightPos(const Vec3& dir);

    void moveLightUpDown(double amount);
    void moveLightRight(double amount);
    void moveLightUp(double amount);
};

Scene::Scene()
{
}

Scene::Scene(int _size_line, int _bytes_per_pixel, void* _image_data)
    : size_line(_size_line), bytes_per_pixel(_bytes_per_pixel), image_data(_image_data), light_pos(Points3(-1, 1, -1)), light_angle(0)
{
    scene_center = Points3(0, 0, -3);
}

Scene::~Scene()
{
}


void Scene::setLightPos(const Points3& pos) {
    this->light_pos = Points3(pos.getX(), pos.getY(), pos.getZ());
}
    
int Scene::compute_lighting(const Vec3& hit, const Vec3& center)
{
    Vec3 normal = Vec3(
        hit.getX() - center.getX(),
        hit.getY() - center.getY(),
        hit.getZ() - center.getZ()
    );
    normal = normalize(normal);

    // Vec3 light_dir = normalize(Vec3(-1, 1, -1));// Vec3(-1, 1, -1) is the direction from the hit point to the light source
    Vec3 light_dir = normalize(light_pos - hit);
    double brightness = std::max(0.0, Vec3::dot(normal, light_dir));
    if (brightness < 0)
        brightness = 0;

    int c = (int)(brightness * 255.0);
    return (c << 16) | (c << 8) | c;// Return grayscale color based on brightness (move the c value to the red, green, and blue channels)
}

void Scene::render(Camera& camera)
{
    int bytes = this->bytes_per_pixel / 8;
    // std::cout << "Rendering scene with camera at (" << camera.getPosition().getX() << ", " << camera.getPosition().getY() << ", " << camera.getPosition().getZ() << ")\n";
    Points3 sphere_center(0, 0, -3);
    double radius = 1.0;

    for (int y = 0; y < IMG_HEIGHT; y++)
    {
        for (int x = 0; x < IMG_WIDTH; x++)
        {
            int offset = y * this->size_line + x * bytes;

            Ray ray = camera.getRay(x, y);

            double t;
            if (intersect_sphere(ray.getOrigin(), ray.getDirection(), sphere_center, radius, t))
            {
                Points3 hit = ray.at(t);
                int color = this->compute_lighting(hit, sphere_center);
                write_pixel(image_data, offset, color);
            }
            else
            {
                write_pixel(image_data, offset, 0x000000FF);
            }
        }
    }
}

// We want to rotate a the light position around an axis passing through the center of the scene.
void Scene::moveLightUpDown(double amount){
    // k is the axe of rotation, in this case we want to rotate around the x axis, so k = (1, 0, 0)
    // this->light_angle += amount;
    this->light_pos = rotateAroundAxis(this->light_pos - scene_center, Vec3(1, 0, 0), amount) + scene_center;

    // v′= vcosθ + (k×v)sinθ+k(k⋅v)(1−cosθ)
}

void Scene::moveLightRight(double amount){
    // k is the axe of rotation, in this case we want to rotate around the y axis, so k = (0, 1, 0)
    // this->light_angle += amount;
    this->light_pos = rotateAroundAxis(this->light_pos - scene_center, Vec3(0, 1, 0), amount) + scene_center;

    // v′= vcosθ + (k×v)sinθ+k(k⋅v)(1−cosθ) 
}