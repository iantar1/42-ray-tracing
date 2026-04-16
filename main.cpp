#include "srcs/Vec3.cpp"
# include "includes/Camera.hpp"
# include "includes/Ray.hpp"
#include "includes/Cylinder.hpp"
# include "includes/Cone.hpp"
extern "C" {
    #include "minilibx-linux/mlx.h"
}

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "includes/Scene.hpp"
# include "includes/Plane.hpp"
#include "includes/Light.hpp"

# define W 119
# define A 97
# define S 115
# define D 100
# define ESC 65307
# define UP 65362
# define DOWN 65364
# define LEFT 65361
# define RIGHT 65363
# define Q 113
# define E 101
# define Z 122
# define X 120
# define R 114
# define T 116
# define Y 121
# define SPACE 32

struct AppState {
    Camera camera;
    Scene scene;
    void* mlx;
    void* win;
    void* img;
};

int close_window(void* param)
{
    Scene* scene = (Scene*)param;
    mlx_destroy_image(scene->mlx, scene->img);
    mlx_destroy_window(scene->mlx, scene->win);
    mlx_destroy_display(scene->mlx);
    free(scene->mlx);
    exit(0);
    return 0;
}

int key_press(int keycode, void* param)
{
    Scene* scene = (Scene*)param;

    // Move camera
    if (keycode == W)
        scene->camera.moveForward(0.1);
    else if (keycode == S)
        scene->camera.moveForward(-0.1);
    else if (keycode == A)
        scene->camera.moveRight(0.1);
    else if (keycode == D)
        scene->camera.moveRight(-0.1);
    else if (keycode == UP)
        scene->moveLightUpDown(0.1);
    else if (keycode == DOWN)
        scene->moveLightUpDown(-0.1);
    else if (keycode == LEFT)
        scene->moveLightRight(-0.1);
    else if (keycode == RIGHT)
        scene->moveLightRight(0.1);
    else if (keycode == SPACE)
    {
        // Switch to next light
        scene->selectNextLight();
        std::cout << "Light " << (scene->getSelectedLightIndex() + 1) << " selected (use arrows to move)" << std::endl;
    }
    // Translate selected object
    else if (keycode == Q)
        scene->translateSelectedObject(Vec3(-0.1, 0, 0));  // Move left
    else if (keycode == E)
        scene->translateSelectedObject(Vec3(0.1, 0, 0));   // Move right
    else if (keycode == Z)
        scene->translateSelectedObject(Vec3(0, 0.1, 0));   // Move up
    else if (keycode == X)
        scene->translateSelectedObject(Vec3(0, -0.1, 0));  // Move down
    // Rotate selected object
    else if (keycode == R)
        scene->rotateSelectedObjectX(0.1);  // Rotate around X-axis
    else if (keycode == T)
        scene->rotateSelectedObjectY(0.1);  // Rotate around Y-axis
    else if (keycode == Y)
        scene->rotateSelectedObjectZ(0.1);  // Rotate around Z-axis
    else if (keycode == ESC)
    {
        close_window(param);
        return 0;
    }

    // Redraw the scene with updated camera
    // render_scene(state->data, state->size_line, state->bpp, state->camera);
    scene->render();
    mlx_put_image_to_window(scene->mlx, scene->win, scene->img, 0, 0);

    return 0;
}





int expose_hook(void* param)
{
    Scene* scene = (Scene*)param;
    mlx_put_image_to_window(scene->mlx, scene->win, scene->img, 0, 0);
    std::cout << "Exposing window\n";
    return 0;
}

int mouse_click(int button, int x, int y, void* param)
{
    Scene* scene = (Scene*)param;
    Objects* obj = scene->getClickedObject(x, y);
    
    if (obj != nullptr)
    {
        std::cout << "Clicked object address: " << obj << std::endl;
        scene->selectObject(obj);
        std::cout << "Object selected." << std::endl;
        std::cout << "  Translation: Q/E (left/right), Z/X (up/down)" << std::endl;
        std::cout << "  Rotation: R (X-axis), T (Y-axis), Y (Z-axis)" << std::endl;
    }
    else
    {
        std::cout << "No object clicked at (" << x << ", " << y << ")" << std::endl;
    }
    return 0;
}

int main()
{
    void* mlx = mlx_init();
    // int bpp, size_line, endian;

    // void* win = mlx_new_window(mlx, IMG_WIDTH, IMG_HEIGHT, (char*)"RT");
    // void* img = mlx_new_image(mlx, IMG_WIDTH, IMG_HEIGHT);
    // void* data = mlx_get_data_addr(img, &bpp, &size_line, &endian);
    // Scene scene(size_line, bpp, data);
    Scene scene(mlx);
    scene.init();
    
    // App state
    // AppState state;
    // state.camera = Camera(Points3(0,0,0), Vec3(0,0,-1), 90.0, (double)IMG_WIDTH / IMG_HEIGHT);
    // state.scene = scene;
    // state.mlx = mlx;
    // state.win = win;
    // state.img = img;
    
    // First render
    // render_scene(state.data, state.size_line, state.bpp, state.camera);
    // scene.addObject(std::make_unique<Sphere>(Sphere(1.0, Points3(0, 0, -3))));
    scene.addObject(std::make_unique<Sphere>(Sphere(1.0, Points3(2, 1, -5), Vec3(255, 0, 0), 64.0, 0.0, 0.6)));  // Reflective red sphere
    // scene.addObject(std::make_unique<Cylinder>(Cylinder(Points3(0, 0, -2), Vec3(0.5, 0.5, 0), 0.5, 1.5)));
    scene.addObject(std::make_unique<Cone>(Cone(Points3(0, -1, -2), Vec3(0, 1, 0), 0.4, 2.0, Vec3(0, 255, 0), 16.0, 0.5)));  // Less shiny green cone
    scene.addObject(std::make_unique<Plane>(Plane(Vec3(0, 0, 1), Points3(2, 1, -6), Vec3(0, 0, 255), 8.0, 0.0, 0.4)));  // Blue reflective plane
    scene.addObject(std::make_unique<Plane>(Plane(Vec3(0, 1, 0), Points3(3, -1, -6), Vec3(255, 255, 0), 32.0, 0.0, 0.7)));  // Reflective yellow plane
    scene.addObject(std::make_unique<Plane>(Plane(Vec3(1, 0, 0), Points3(-2, 0, -1), Vec3(255, 0, 255), 4.0)));  // Non-reflective magenta plane

    // Add multiple lights
    scene.addLight(Light(1.0, 0, Vec3(-1, 1, -1)));
    scene.addLight(Light(0.8, 0, Vec3(2, 2, -2)));
    scene.addLight(Light(0.6, 0, Vec3(-2, -1, -3)));

    scene.render();
    mlx_put_image_to_window(scene.mlx, scene.win, scene.img, 0, 0);
    
    // Hook keys
    mlx_hook(scene.win, 2, 1L<<0, (int (*)())key_press, &scene);
    mlx_hook(scene.win, 4, 1L<<2, (int (*)())mouse_click, &scene);
    mlx_hook(scene.win, 17, 0, (int (*)())close_window, &scene);
    // Expose event (when the window must be repainted). Mask must not be 0.
    mlx_hook(scene.win, 12, 1L<<15, (int (*)())expose_hook, &scene);

    mlx_loop(scene.mlx);
}