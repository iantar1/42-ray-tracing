#include "srcs/Vec3.cpp"
#include "includes/Camera.hpp"
#include "includes/Ray.hpp"
#include "includes/Cylinder.hpp"
#include "includes/Cone.hpp"
#include "includes/Square.hpp"
#include "includes/Triangle.hpp"
extern "C" {
    #include "minilibx-linux/mlx.h"
}

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <chrono>
#include "includes/Scene.hpp"
#include "includes/Plane.hpp"
#include "includes/Light.hpp"
#include "includes/Parser.hpp"

#ifdef MACOS
# define W     13
# define A     0
# define S     1
# define D     2
# define ESC   53
# define UP    126
# define DOWN  125
# define LEFT  123
# define RIGHT 124
# define Q     12
# define E     14
# define Z     6
# define X_KEY 7
# define R_KEY 15
# define T_KEY 17
# define Y_KEY 16
# define SPACE 49
#else
# define W     119
# define A     97
# define S     115
# define D     100
# define ESC   65307
# define UP    65362
# define DOWN  65364
# define LEFT  65361
# define RIGHT 65363
# define Q     113
# define E     101
# define Z     122
# define X_KEY 120
# define R_KEY 114
# define T_KEY 116
# define Y_KEY 121
# define SPACE 32
#endif

static char g_render_buf[64] = "";

void render_and_display(Scene* scene)
{
    auto t0 = std::chrono::high_resolution_clock::now();
    scene->render();
    auto t1 = std::chrono::high_resolution_clock::now();
    double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();

    mlx_put_image_to_window(scene->mlx, scene->win, scene->img, 0, 0);

    snprintf(g_render_buf, sizeof(g_render_buf), "Render: %.1f ms (%.1f fps)", ms, 1000.0 / ms);
    mlx_string_put(scene->mlx, scene->win, 10, 20, 0xFFFFFF, g_render_buf);
}

int close_window(void* param)
{
    Scene* scene = (Scene*)param;
    mlx_destroy_image(scene->mlx, scene->img);
    mlx_destroy_window(scene->mlx, scene->win);
    
    free(scene->mlx);
    exit(0);
    return 0;
}

int key_press(int keycode, void* param)
{
    Scene* scene = (Scene*)param;

    if (keycode == W)
        scene->camera.moveForward(0.1);
    else if (keycode == S)
        scene->camera.moveForward(-0.1);
    else if (keycode == A)
        scene->camera.moveRight(-0.1);
    else if (keycode == D)
        scene->camera.moveRight(0.1);
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
        scene->selectNextLight();
        std::cout << "Light " << (scene->getSelectedLightIndex() + 1) << " selected\n";
    }
    else if (keycode == Q)
        scene->translateSelectedObject(Vec3(-0.1, 0, 0));
    else if (keycode == E)
        scene->translateSelectedObject(Vec3(0.1, 0, 0));
    else if (keycode == Z)
        scene->translateSelectedObject(Vec3(0, 0.1, 0));
    else if (keycode == X_KEY)
        scene->translateSelectedObject(Vec3(0, -0.1, 0));
    else if (keycode == R_KEY)
        scene->rotateSelectedObjectX(0.1);
    else if (keycode == T_KEY)
        scene->rotateSelectedObjectY(0.1);
    else if (keycode == Y_KEY)
        scene->rotateSelectedObjectZ(0.1);
    else if (keycode == ESC)
    {
        close_window(param);
        return 0;
    }

    render_and_display(scene);
    return 0;
}

int expose_hook(void* param)
{
    Scene* scene = (Scene*)param;
    mlx_put_image_to_window(scene->mlx, scene->win, scene->img, 0, 0);
    if (g_render_buf[0])
        mlx_string_put(scene->mlx, scene->win, 10, 20, 0xFFFFFF, g_render_buf);
    return 0;
}

int mouse_click(int button, int x, int y, void* param)
{
    (void)button;
    Scene* scene = (Scene*)param;
    Objects* obj = scene->getClickedObject(x, y);
    if (obj != nullptr)
    {
        scene->selectObject(obj);
        std::cout << "Object selected. Q/E: X  Z/X: Y  R/T/Y: rotate\n";
    }
    return 0;
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <scene.rt>\n";
        return 1;
    }

    void* mlx = mlx_init();
    Scene scene(mlx);

    try
    {
        Parser parser(argv[1]);
        scene.setDimensions(parser.width, parser.height);
        scene.init();
        parser.apply(scene);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    render_and_display(&scene);

    mlx_hook(scene.win, 2,  1L<<0,  (int (*)())key_press,    &scene);
    mlx_hook(scene.win, 4,  1L<<2,  (int (*)())mouse_click,  &scene);
    mlx_hook(scene.win, 17, 0,      (int (*)())close_window,  &scene);
    mlx_hook(scene.win, 12, 1L<<15, (int (*)())expose_hook,   &scene);

    mlx_loop(scene.mlx);
}
