#pragma once
#include <vector>
#include "Sphere.hpp"
#include "Camera.hpp"
#include "utils.hpp"
#include <algorithm>
# include "Objects.hpp"
#include <memory>

# define BACKGROUND_COLOR 0x0000FF

class Scene
{
private:
    // it must contain class of types objects, and other objects will inherit from it.
    std::vector<std::unique_ptr<Objects>> objects;
    Points3 scene_center;
    Points3 light_pos;
    double light_angle;
    int bytes_per_pixel;
    void* image_data;
    
    int bpp;
    int size_line;
    int     endian;
    // void* data;
    
    // std::vector<Objects> objects;
    bool isShadowed(const Points3& hit, const Vec3& light_dir, const Objects* exclude_obj);
    
public:
    void*   mlx;
    void*   win;
    void*   img;

    Scene();
    Scene(const Scene&) = delete;// Delete copy — Scene cannot be copied (because unique_ptr can't)
    Scene& operator=(const Scene&) = delete;
    Scene(int _size_line, int _bytes_per_pixel, void* _image_data);
    Scene(void*);
    ~Scene();
    Camera camera;
    
    
    int computeLighting(const Vec3& hit, const Sphere& obj);
    void render();
    void addObject(std::unique_ptr<Objects> obj);
    int compute_lighting(const Vec3& hit,  std::unique_ptr<Objects>& obj);
    void setLightPos(const Vec3& dir);

    void moveLightUpDown(double amount);
    void moveLightRight(double amount);
    void moveLightUp(double amount);

    void    init();//put all the shit that creaet the win and img ... just cut and past code form main
};

Scene::Scene()
{
}

Scene::Scene(void* _mlx): mlx(_mlx), light_pos(Points3(-1, 1, -1)), scene_center(Points3(0, 0, -3)), light_angle(0), bytes_per_pixel(0)
{
}

void    Scene::init() {
    this->win = mlx_new_window(mlx, IMG_WIDTH, IMG_HEIGHT, (char*)"RT");
    this->img = mlx_new_image(mlx, IMG_WIDTH, IMG_HEIGHT);
    this->image_data = mlx_get_data_addr(img, &bpp, &size_line, &endian);
    this->camera = Camera(Points3(0,0,0), Vec3(0,0,-1), 90.0, (double)IMG_WIDTH / IMG_HEIGHT);
}

// Scene::Scene(int _size_line, int _bytes_per_pixel, void* _image_data)
//     : size_line(_size_line), bytes_per_pixel(_bytes_per_pixel), image_data(_image_data), light_pos(Points3(-1, 1, -1)), light_angle(0)
// {
//     scene_center = Points3(0, 0, -3);
// }

Scene::~Scene()
{
}

bool Scene::isShadowed(const Points3& hit, const Vec3& light_dir, const Objects* exclude_obj)
{
    // Create a ray from hit point toward the light
    // Add a small epsilon offset to avoid self-intersection
    const double epsilon = 1e-4;
    Points3 shadow_ray_origin = Points3(
        hit.getX() + light_dir.getX() ,
        hit.getY() + light_dir.getY() ,
        hit.getZ() + light_dir.getZ() 
    );
    
    Ray shadow_ray(shadow_ray_origin, light_dir);
    
    // Calculate distance to light source
    Vec3 to_light = light_pos - hit;
    double distance_to_light = sqrt(Vec3::dot(to_light, to_light));
    
    // Check intersection with all objects except the one we just hit
    for (const std::unique_ptr<Objects>& obj : objects)
    {
        // Skip the object that was hit (to avoid self-shadowing)
        if (obj.get() == exclude_obj)
            continue;
            
        double t;
        if (obj->intersect(shadow_ray, t))
        {
            // Only consider it a shadow if the intersection is between hit point and light
            // and closer than the light source
            if (t > epsilon && t < distance_to_light)
                return true;
        }
    }
    return false;

}

void Scene::setLightPos(const Points3& pos) {
    this->light_pos = Points3(pos.getX(), pos.getY(), pos.getZ());
}
    
int Scene::compute_lighting(const Vec3& hit, std::unique_ptr<Objects>& obj)
{
    Vec3 normal = obj->get_normal(hit);
    normal = normalize(normal);

    // Vec3 light_dir = normalize(Vec3(-1, 1, -1));// Vec3(-1, 1, -1) is the direction from the hit point to the light source
    Vec3 light_dir = normalize(light_pos - hit);

    if (isShadowed(hit, light_dir, obj.get()))
    {
        // Use ambient light instead of complete black
        int ambient_c = (int)(0.2 * 255.0); // 20% ambient brightness
        return (ambient_c << 16) | (ambient_c << 8) | ambient_c;
    }
    double brightness = std::max(0.0, Vec3::dot(normal, light_dir));
    if (brightness < 0)
        brightness = 0;

    int c = (int)(brightness * 255.0);
    return (c << 16) | (c << 8) | c;// Return grayscale color based on brightness (move the c value to the red, green, and blue channels)
}

void Scene::render()
{
    int bytes = this->bpp / 8;

    for (int y = 0; y < IMG_HEIGHT; y++)
    {
        for (int x = 0; x < IMG_WIDTH; x++)
        {
            int offset = y * this->size_line + x * bytes;
            int color = BACKGROUND_COLOR;
            double closest_t = std::numeric_limits<double>::max();
            for (std::unique_ptr<Objects>& obj : objects)
            {
                Ray ray = camera.getRay(x, y);
    
                double t;
                if (obj->intersect(ray, t))
                {
                    Points3 hit = ray.at(t);
                    if (t < closest_t)
                    {
                        closest_t = t;
                        color = this->compute_lighting(hit, obj);
                    }
                }
            }
            write_pixel(image_data, offset, color);
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

void Scene::addObject(std::unique_ptr<Objects> obj)
{
    objects.push_back(std::move(obj));
}