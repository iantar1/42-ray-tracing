#pragma once
#include <vector>
#include "Sphere.hpp"
#include "Camera.hpp"
#include "utils.hpp"
#include <algorithm>
# include "Objects.hpp"
#include <memory>
#include "Light.hpp"

# define BACKGROUND_COLOR 0x0000FF

class Scene
{
private:
    // it must contain class of types objects, and other objects will inherit from it.
    std::vector<std::unique_ptr<Objects>> objects;
    std::vector<Light> lights;
    Points3 scene_center;
    double light_angle;
    int bytes_per_pixel;
    void* image_data;
    
    int bpp;
    int size_line;
    int     endian;
    // void* data;
    
    // std::vector<Objects> objects;
    Objects* selected_object;
    int selected_light_index;
    bool isShadowed(const Points3& hit, const Vec3& light_dir, const Objects* exclude_obj, const Light& light);
    Vec3 cast_ray(const Ray& ray, int depth);
    int compute_lighting_with_reflection(const Vec3& hit, std::unique_ptr<Objects>& obj, int depth);
    
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
    void addLight(const Light& light);

    void moveLightUpDown(double amount);
    void moveLightRight(double amount);
    void moveLightUp(double amount);
    void selectNextLight();
    int getSelectedLightIndex() const;

    Objects* getClickedObject(int x, int y);
    void selectObject(Objects* obj);
    Objects* getSelectedObject() const;
    void translateSelectedObject(const Vec3& offset);
    void rotateSelectedObjectX(double angle);
    void rotateSelectedObjectY(double angle);
    void rotateSelectedObjectZ(double angle);

    void    init();//put all the shit that creaet the win and img ... just cut and past code form main
};