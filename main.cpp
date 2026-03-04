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
#include "includes/Scene.hpp"

# define W 119
# define A 97
# define S 115
# define D 100
# define ESC 65307
# define UP 65362
# define DOWN 65364
# define LEFT 65361
# define RIGHT 65363

struct AppState {
    Camera camera;
    Scene scene;
    void* mlx;
    void* win;
    void* img;
};

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
    else if (keycode == UP)
        state->scene.moveLightUpDown(0.1);
    else if (keycode == DOWN)
        state->scene.moveLightUpDown(-0.1);
    else if (keycode == LEFT)
        state->scene.moveLightRight(-0.1);
    else if (keycode == RIGHT)
        state->scene.moveLightRight(0.1);
    else if (keycode == ESC)
        exit(0);

    // Redraw the scene with updated camera
    // render_scene(state->data, state->size_line, state->bpp, state->camera);
    state->scene.render(state->camera);
    mlx_put_image_to_window(state->mlx, state->win, state->img, 0, 0);

    return 0;
}


int close_window()
{
    exit(0);
    return 0;
}



int main()
{
    void* mlx = mlx_init();
    int bpp, size_line, endian;

    void* win = mlx_new_window(mlx, IMG_WIDTH, IMG_HEIGHT, (char*)"RT");
    void* img = mlx_new_image(mlx, IMG_WIDTH, IMG_HEIGHT);
    void* data = mlx_get_data_addr(img, &bpp, &size_line, &endian);
    Scene scene(size_line, bpp, data);

    // App state
    AppState state;
    state.camera = Camera(Points3(0,0,0), Vec3(0,0,-1), 90.0, (double)IMG_WIDTH / IMG_HEIGHT);
    state.scene = scene;
    state.mlx = mlx;
    state.win = win;
    state.img = img;

    // First render
    // render_scene(state.data, state.size_line, state.bpp, state.camera);
    scene.render(state.camera);
    mlx_put_image_to_window(mlx, win, img, 0, 0);

    // Hook keys
    mlx_hook(win, 2, 1L<<0, (int (*)())key_press, &state);
    mlx_hook(win, 17, 0, close_window, NULL);

    mlx_loop(mlx);
}