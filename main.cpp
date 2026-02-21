#include "srcs/Vec3.cpp"
# include "includes/Camera.hpp"
# include "includes/Ray.hpp"
extern "C" {
    #include "minilibx-linux/mlx.h"
}

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

# define W 119
# define A 97
# define S 115
# define D 100
# define ESC 65307


struct AppState {
    Camera camera;
    void* mlx;
    void* win;
    void* img;
    void* data;
    int bpp;
    int size_line;
};


/* ========================= UTILS ========================= */


static inline void write_pixel(void* data, int offset, int color)
{
    *(unsigned int*)((char*)data + offset) = color;
}

/* ========================= RAY ========================= */

Vec3 pixel_to_ray(int x, int y)
{
    double px = (2.0 * x / IMG_WIDTH) - 1.0;
    double py = 1.0 - (2.0 * y / IMG_HEIGHT);

    return Vec3(px, py, -1);
}

/* ========================= SPHERE ========================= */

bool intersect_sphere(
    const Vec3& ray_origin,
    const Vec3& ray_dir,
    const Vec3& center,
    double radius,
    double& t_hit
)
{
    // Compute the vector from ray origin to sphere center oc = ray_origin - center
    Vec3 oc = Vec3(
        ray_origin.getX() - center.getX(),
        ray_origin.getY() - center.getY(),
        ray_origin.getZ() - center.getZ()
    );
// sphere equation: (P - C) . (P - C) = r^2
// ray equation: P = O + tD
// substitute ray into sphere: (O + tD - C) . (O + tD - C) = r^2
// expand: (O - C) . (O - C) + 2tD . (O - C) + t^2 D . D = r^2
// rearrange: t^2 D . D + 2t D . (O - C) + (O - C) . (O - C) - r^2 = 0
// This is a quadratic equation in t: at^2 + bt + c = 0
// where: a = D . D, b = 2 D . (O - C), c = (O - C) .(O - C) - r^2
// and ray_dir is D, oc is (O - C)

    double a = Vec3::dot(ray_dir, ray_dir);
    double b = 2.0 * Vec3::dot(oc, ray_dir);
    double c = Vec3::dot(oc, oc) - radius*radius;

    double disc = b*b - 4*a*c;

    // No intersection if discriminant is negative
    // one solution: inside the sphere, two solutions: outside the sphere
    if (disc <= 0)
        return false;

    double t1 = (-b - sqrt(disc)) / (2.0 * a);
    double t2 = (-b + sqrt(disc)) / (2.0 * a);
    if (t1 > 0)
        t_hit = t1;
    else if (t2 > 0)
        t_hit = t2;
    else
        return false; // Both intersections are behind the camera
    // t_hit = (-b - sqrt(disc)) / (2.0 * a);
    return true;
}

/* ========================= SHADING ========================= */

int compute_lighting(const Vec3& hit, const Vec3& center)
{
    Vec3 normal = Vec3(
        hit.getX() - center.getX(),
        hit.getY() - center.getY(),
        hit.getZ() - center.getZ()
    );
    normal = normalize(normal);

    Vec3 light_dir = normalize(Vec3(-1, 1, -1));// Vec3(-1, 1, -1) is the direction from the hit point to the light source

    double brightness = Vec3::dot(normal, light_dir);
    if (brightness < 0)
        brightness = 0;

    int c = (int)(brightness * 255.0);
    return (c << 16) | (c << 8) | c;// Return grayscale color based on brightness (move the c value to the red, green, and blue channels)
}

/* ========================= RENDER ========================= */

void render_scene(void* data, int size_line, int bpp, Camera& camera)
{
    int bytes = bpp / 8;
    std::cout << "Rendering scene with camera at (" << camera.getPosition().getX() << ", " << camera.getPosition().getY() << ", " << camera.getPosition().getZ() << ")\n";
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
                write_pixel(data, offset, color);
            }
            else
            {
                write_pixel(data, offset, 0x000000FF);
            }
        }
    }
}


int key_press(int keycode, void* param)
{
    AppState* state = (AppState*)param;

    // Move camera
    if (keycode == W)
        state->camera.moveForward(0.1);
    else if (keycode == S)
        state->camera.moveForward(-0.1);
    else if (keycode == A)
        state->camera.moveRight(0.1);
    else if (keycode == D)
        state->camera.moveRight(-0.1);
    else if (keycode == ESC)
        exit(0);

    // Redraw the scene with updated camera
    render_scene(state->data, state->size_line, state->bpp, state->camera);
    mlx_put_image_to_window(state->mlx, state->win, state->img, 0, 0);

    return 0;
}





// void render(void* data, int size_line, int bpp, void* win)
// {
//     int bytes = bpp / 8;


//      // Ray from camera origin pointing into the scene
//     Points3 sphere_center = Points3(0,0,-3);
//     Camera camera(Points3(0,0,0), Vec3(0,0,-1), 90.0, (double)IMG_WIDTH / IMG_HEIGHT);
//     double radius = 1.0;

//     for (int y = 0; y < IMG_HEIGHT; y++)
//     {
//         for (int x = 0; x < IMG_WIDTH; x++)
//         {
//             int offset = y*size_line + x*bytes;
            
//             Ray ray = camera.getRay(x, y);
            
//             double t;
//             if (intersect_sphere(ray.getOrigin(), ray.getDirection(), sphere_center, radius, t))
//             {
//                 Points3 hit = ray.at(t);
                
//                 int color = compute_lighting(hit, sphere_center);
//                 write_pixel(data, offset, color);
//             }
//             else
//             {
//                 write_pixel(data, offset, 0x000000FF);
//             }
//         }
//     }
//     mlx_hook(win, 2, 1L<<0, (int (*)())key_press, &camera);
// }

/* ========================= MLX ========================= */

int close_window()
{
    exit(0);
    return 0;
}



int main()
{
    void* mlx = mlx_init();
    int bpp, size_line, endian;

    void* win = mlx_new_window(mlx, IMG_WIDTH, IMG_HEIGHT, (char*)"MiniRT");
    void* img = mlx_new_image(mlx, IMG_WIDTH, IMG_HEIGHT);
    void* data = mlx_get_data_addr(img, &bpp, &size_line, &endian);

    // App state
    AppState state;
    state.camera = Camera(Points3(0,0,0), Vec3(0,0,-1), 90.0, (double)IMG_WIDTH / IMG_HEIGHT);
    state.mlx = mlx;
    state.win = win;
    state.img = img;
    state.data = data;
    state.bpp = bpp;
    state.size_line = size_line;

    // First render
    render_scene(state.data, state.size_line, state.bpp, state.camera);
    mlx_put_image_to_window(mlx, win, img, 0, 0);

    // Hook keys
    mlx_hook(win, 2, 1L<<0, (int (*)())key_press, &state);
    mlx_hook(win, 17, 0, close_window, NULL);

    mlx_loop(mlx);
}