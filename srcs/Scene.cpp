#include "../includes/Scene.hpp"
#include <limits>

extern "C" {
#include "../minilibx-linux/mlx.h"
}

Scene::Scene()
{
    selected_object = nullptr;
    selected_light_index = 0;
}

Scene::Scene(void* _mlx): mlx(_mlx), scene_center(Points3(0, 0, -3)), light_angle(0), bytes_per_pixel(0)
{
    selected_object = nullptr;
    selected_light_index = 0;
}

void Scene::init() {
    this->win = mlx_new_window(mlx, IMG_WIDTH, IMG_HEIGHT, (char*)"RT");
    this->img = mlx_new_image(mlx, IMG_WIDTH, IMG_HEIGHT);
    this->image_data = mlx_get_data_addr(img, &bpp, &size_line, &endian);
    this->camera = Camera(Points3(0,0,0), Vec3(0,0,-1), 90.0, (double)IMG_WIDTH / IMG_HEIGHT);
}

Scene::~Scene()
{
}

bool Scene::isShadowed(const Points3& hit, const Vec3& light_dir, const Objects* exclude_obj, const Light& light)
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
    Vec3 to_light = light.getPosition() - hit;
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
    if (!lights.empty()) {
        lights[0] = Light(1.0, 0, Vec3(pos.getX(), pos.getY(), pos.getZ()));
    }
}

void Scene::addLight(const Light& light) {
    lights.push_back(light);
}
    
int Scene::compute_lighting(const Vec3& hit, std::unique_ptr<Objects>& obj)
{
    Vec3 normal = obj->get_normal(hit);
    normal = normalize(normal);

    Vec3 obj_color = obj->getColor();
    double shininess = obj->getShininess();
    
    // If no lights, use ambient light only
    if (lights.empty()) {
        int ambient_c = (int)(0.2 * 255.0);
        int r = (int)(obj_color.getX() * ambient_c / 255.0);
        int g = (int)(obj_color.getY() * ambient_c / 255.0);
        int b = (int)(obj_color.getZ() * ambient_c / 255.0);
        return (r << 16) | (g << 8) | b;
    }
    
    // Accumulate light contributions from all lights
    double diffuse_brightness = 0.0;
    double specular_brightness = 0.0;
    
    // View direction (from hit point to camera at origin)
    Vec3 view_dir = normalize(camera.getPosition() - hit);
    
    for (const Light& light : lights) {
        Vec3 light_dir = normalize(light.getPosition() - hit);
        
        if (isShadowed(hit, light_dir, obj.get(), light)) {
            // Use ambient light instead of complete black
            diffuse_brightness += 0.2 * light.getIntensity();
        } else {
            // Diffuse component: Lambert's law (N · L)
            double diffuse = std::max(0.0, Vec3::dot(normal, light_dir));
            diffuse_brightness += diffuse * light.getIntensity();
            
            // Specular component: Phong reflection model
            // R = 2(N · L)N - L  (reflected light direction)
            Vec3 reflection = normal * (2.0 * Vec3::dot(normal, light_dir)) - light_dir;
            reflection = normalize(reflection);
            
            // Specular = (R · V)^shininess
            double specular = std::max(0.0, Vec3::dot(reflection, view_dir));
            specular = std::pow(specular, shininess);
            specular_brightness += specular * light.getIntensity();
        }
    }
    
    // Clamp diffuse brightness to avoid oversaturation
    if (diffuse_brightness > 1.0)
        diffuse_brightness = 1.0;
    
    // Clamp specular brightness (typically much smaller contribution)
    if (specular_brightness > 1.0)
        specular_brightness = 1.0;

    // Combine diffuse (colored by object) and specular (white highlight)
    int r = (int)(diffuse_brightness * obj_color.getX() + specular_brightness * 255.0);
    int g = (int)(diffuse_brightness * obj_color.getY() + specular_brightness * 255.0);
    int b = (int)(diffuse_brightness * obj_color.getZ() + specular_brightness * 255.0);
    
    // Clamp final RGB values to 0-255
    r = (r > 255) ? 255 : r;
    g = (g > 255) ? 255 : g;
    b = (b > 255) ? 255 : b;
    
    return (r << 16) | (g << 8) | b;
}

int Scene::compute_lighting_with_reflection(const Vec3& hit, std::unique_ptr<Objects>& obj, int depth)
{
    Vec3 normal = obj->get_normal(hit);
    normal = normalize(normal);

    Vec3 obj_color = obj->getColor();
    double shininess = obj->getShininess();
    double reflectivity = obj->getReflectivity();
    
    // If no lights, use ambient light only
    if (lights.empty()) {
        int ambient_c = (int)(0.2 * 255.0);
        int r = (int)(obj_color.getX() * ambient_c / 255.0);
        int g = (int)(obj_color.getY() * ambient_c / 255.0);
        int b = (int)(obj_color.getZ() * ambient_c / 255.0);
        return (r << 16) | (g << 8) | b;
    }
    
    // Accumulate light contributions from all lights
    double diffuse_brightness = 0.0;
    double specular_brightness = 0.0;
    
    // View direction (from hit point to camera at origin)
    Vec3 view_dir = normalize(camera.getPosition() - hit);
    
    for (const Light& light : lights) {
        Vec3 light_dir = normalize(light.getPosition() - hit);
        
        if (isShadowed(hit, light_dir, obj.get(), light)) {
            // Use ambient light instead of complete black
            diffuse_brightness += 0.2 * light.getIntensity();
        } else {
            // Diffuse component: Lambert's law (N · L)
            double diffuse = std::max(0.0, Vec3::dot(normal, light_dir));
            diffuse_brightness += diffuse * light.getIntensity();
            
            // Specular component: Phong reflection model
            // R = 2(N · L)N - L  (reflected light direction)
            Vec3 reflection = normal * (2.0 * Vec3::dot(normal, light_dir)) - light_dir;
            reflection = normalize(reflection);
            
            // Specular = (R · V)^shininess
            double specular = std::max(0.0, Vec3::dot(reflection, view_dir));
            specular = std::pow(specular, shininess);
            specular_brightness += specular * light.getIntensity();
        }
    }
    
    // Clamp diffuse brightness to avoid oversaturation
    if (diffuse_brightness > 1.0)
        diffuse_brightness = 1.0;
    
    // Clamp specular brightness (typically much smaller contribution)
    if (specular_brightness > 1.0)
        specular_brightness = 1.0;

    // Base color from direct lighting
    int r = (int)(diffuse_brightness * obj_color.getX() + specular_brightness * 255.0);
    int g = (int)(diffuse_brightness * obj_color.getY() + specular_brightness * 255.0);
    int b = (int)(diffuse_brightness * obj_color.getZ() + specular_brightness * 255.0);
    
    // Add reflection if object is reflective and we haven't exceeded max depth
    if (reflectivity > 0.0 && depth < 3)
    {
        // Calculate reflection ray: R = V - 2(V · N)N
        Vec3 hit_vec(hit.getX(), hit.getY(), hit.getZ());
        Vec3 camera_pos = camera.getPosition();
        Vec3 incoming_dir = normalize(hit_vec - camera_pos);  // Direction from camera to hit
        Vec3 reflected_dir = incoming_dir - normal * (2.0 * Vec3::dot(incoming_dir, normal));
        reflected_dir = normalize(reflected_dir);
        
        const double epsilon = 1e-4;
        Points3 reflection_origin = Points3(
            hit.getX() + normal.getX() * epsilon,
            hit.getY() + normal.getY() * epsilon,
            hit.getZ() + normal.getZ() * epsilon
        );
        
        Ray reflection_ray(reflection_origin, reflected_dir);
        Vec3 reflected_color = cast_ray(reflection_ray, depth + 1);
        
        // Blend reflected color with direct lighting
        r = (int)(r * (1.0 - reflectivity) + reflected_color.getX() * reflectivity);
        g = (int)(g * (1.0 - reflectivity) + reflected_color.getY() * reflectivity);
        b = (int)(b * (1.0 - reflectivity) + reflected_color.getZ() * reflectivity);
    }
    
    // Clamp final RGB values to 0-255
    r = (r > 255) ? 255 : r;
    g = (g > 255) ? 255 : g;
    b = (b > 255) ? 255 : b;
    
    return (r << 16) | (g << 8) | b;
}

Vec3 Scene::cast_ray(const Ray& ray, int depth)
{
    // Background color as RGB normalized to 0-1 range
    Vec3 background = Vec3(0.0, 0.0, 1.0);  // Blue background
    
    if (depth >= 3)
        return background;
    
    double closest_t = std::numeric_limits<double>::max();
    Objects* hit_obj = nullptr;
    
    for (std::unique_ptr<Objects>& obj : objects)
    {
        double t;
        if (obj->intersect(ray, t))
        {
            if (t < closest_t && t > 0)
            {
                closest_t = t;
                hit_obj = obj.get();
            }
        }
    }
    
    // No hit, return background
    if (hit_obj == nullptr)
        return background;
    
    // Calculate hit point and lighting
    Points3 hit = ray.at(closest_t);
    
    // Find the matching unique_ptr for the hit object to pass to compute_lighting_with_reflection
    for (std::unique_ptr<Objects>& obj : objects)
    {
        if (obj.get() == hit_obj)
        {
            int color = compute_lighting_with_reflection(hit, obj, depth);
            int r = (color >> 16) & 0xFF;
            int g = (color >> 8) & 0xFF;
            int b = color & 0xFF;
            return Vec3(r, g, b);
        }
    }
    
    return background;
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
            Ray ray = camera.getRay(x, y);
            
            // Use the recursive ray casting for reflection support
            Vec3 ray_color = cast_ray(ray, 0);
            
            int r = (int)ray_color.getX();
            int g = (int)ray_color.getY();
            int b = (int)ray_color.getZ();
            
            // Clamp values
            r = (r > 255) ? 255 : (r < 0) ? 0 : r;
            g = (g > 255) ? 255 : (g < 0) ? 0 : g;
            b = (b > 255) ? 255 : (b < 0) ? 0 : b;
            
            color = (r << 16) | (g << 8) | b;
            write_pixel(image_data, offset, color);
        }
    }

}

// We want to rotate a the light position around an axis passing through the center of the scene.
void Scene::moveLightUpDown(double amount){
    // k is the axe of rotation, in this case we want to rotate around the x axis, so k = (1, 0, 0)
    // this->light_angle += amount;
    if (selected_light_index < lights.size()) {
        Vec3 light_pos = lights[selected_light_index].getPosition();
        light_pos = rotateAroundAxis(light_pos - scene_center, Vec3(1, 0, 0), amount) + scene_center;
        lights[selected_light_index] = Light(lights[selected_light_index].getIntensity(), 0, light_pos);
    }
    // v′= vcosθ + (k×v)sinθ+k(k⋅v)(1−cosθ)
}

void Scene::moveLightRight(double amount){
    // k is the axe of rotation, in this case we want to rotate around the y axis, so k = (0, 1, 0)
    // this->light_angle += amount;
    if (selected_light_index < lights.size()) {
        Vec3 light_pos = lights[selected_light_index].getPosition();
        light_pos = rotateAroundAxis(light_pos - scene_center, Vec3(0, 1, 0), amount) + scene_center;
        lights[selected_light_index] = Light(lights[selected_light_index].getIntensity(), 0, light_pos);
    }
    // v′= vcosθ + (k×v)sinθ+k(k⋅v)(1−cosθ) 
}

void Scene::moveLightUp(double amount){
    // Move light up in world space (along Y axis)
    if (selected_light_index < lights.size()) {
        Vec3 light_pos = lights[selected_light_index].getPosition();
        light_pos = light_pos + Vec3(0, amount, 0);
        lights[selected_light_index] = Light(lights[selected_light_index].getIntensity(), 0, light_pos);
    }
}

void Scene::selectNextLight() {
    if (!lights.empty()) {
        selected_light_index = (selected_light_index + 1) % lights.size();
    }
}

int Scene::getSelectedLightIndex() const {
    return selected_light_index;
}

void Scene::addObject(std::unique_ptr<Objects> obj)
{
    objects.push_back(std::move(obj));
}

Objects* Scene::getClickedObject(int x, int y)
{
    Ray ray = camera.getRay(x, y);
    double closest_t = std::numeric_limits<double>::max();
    Objects* clicked_obj = nullptr;
    
    for (std::unique_ptr<Objects>& obj : objects)
    {
        double t;
        if (obj->intersect(ray, t))
        {
            if (t < closest_t && t > 0)
            {
                closest_t = t;
                clicked_obj = obj.get();
            }
        }
    }
    return clicked_obj;
}

void Scene::selectObject(Objects* obj)
{
    selected_object = obj;
}

Objects* Scene::getSelectedObject() const
{
    return selected_object;
}

void Scene::translateSelectedObject(const Vec3& offset)
{
    if (selected_object != nullptr)
    {
        selected_object->translate(offset);
    }
}

void Scene::rotateSelectedObjectX(double angle)
{
    if (selected_object != nullptr)
    {
        selected_object->rotateX(angle);
    }
}

void Scene::rotateSelectedObjectY(double angle)
{
    if (selected_object != nullptr)
    {
        selected_object->rotateY(angle);
    }
}

void Scene::rotateSelectedObjectZ(double angle)
{
    if (selected_object != nullptr)
    {
        selected_object->rotateZ(angle);
    }
}
