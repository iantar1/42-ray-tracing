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
#include "srcs/utils.cpp"

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




/* ========================= RAY ========================= */

Vec3 pixel_to_ray(int x, int y)
{
    double px = (2.0 * x / IMG_WIDTH) - 1.0;
    double py = 1.0 - (2.0 * y / IMG_HEIGHT);

    return Vec3(px, py, -1);
}

/* ========================= SPHERE ========================= */



/* ========================= SHADING ========================= */


/* ========================= RENDER ========================= */

void render_scene(void* data, int size_line, int bpp, Camera& camera)
{
    int bytes = bpp / 8;
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