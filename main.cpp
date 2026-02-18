
// #include "includes/Constants.hpp"
#include "srcs/Vec3.cpp"
// #include "includes/Camera.hpp"
# define IMG_WIDTH 800
# define IMG_HEIGHT 600

extern "C" {
    #include "minilibx-linux/mlx.h"
}
# include <stdlib.h>
# include <math.h>


int    close_window()
{
    exit(9);
    return 0;
}

// x ^ 2 + y ^ 2 = r ^ 2
bool is_inside_circle(int x, int y, int radius)
{
    return (x * x + y * y) <= (radius * radius);
}

bool is_inside_sphere(int x, int y, int z, int radius)
{
    return (x * x + y * y + z * z) <= (radius * radius);
}

void render(void* data, int size_line, int bits_per_pixel)
{
    int bytes_per_pixel = bits_per_pixel / 8;

    Vec3 sphere_center = Vec3(0, 0, -3);
    double sphere_radius = 1.0;

    for (int y = 0; y < IMG_HEIGHT; y++)
    {
        for (int x = 0; x < IMG_WIDTH; x++)
        {
            int offset = (y * size_line) + (x * bytes_per_pixel);

            // Convert pixel to normalized screen space (-1 to 1)
            double px = (2.0 * x / IMG_WIDTH) - 1.0; // instead of 0 -> IMG_WIDTH, we want -1 -> 1
            double py = 1.0 - (2.0 * y / IMG_HEIGHT); // Invert y-axis for screen space

            Vec3 ray_origin = Vec3(0, 0, 0);
            Vec3 ray_dir = Vec3(px, py, -1);

            // Ray-sphere intersection
            Vec3 oc = Vec3(
                ray_origin.getX() - sphere_center.getX(),
                ray_origin.getY() - sphere_center.getY(),
                ray_origin.getZ() - sphere_center.getZ()
            );

            double a = Vec3::dot(ray_dir, ray_dir);
            double b = 2.0 * Vec3::dot(oc, ray_dir);
            double c = Vec3::dot(oc, oc) - sphere_radius * sphere_radius;

            double discriminant = b*b - 4*a*c;

            if (discriminant > 0) {
                double t = (-b - sqrt(discriminant)) / (2.0 * a);

                // Compute hit point
                Vec3 hit = Vec3(
                    ray_origin.getX() + t * ray_dir.getX(),
                    ray_origin.getY() + t * ray_dir.getY(),
                    ray_origin.getZ() + t * ray_dir.getZ()
                );

                // Surface normal = (hit - center)
                Vec3 normal = Vec3(
                    hit.getX() - sphere_center.getX(),
                    hit.getY() - sphere_center.getY(),
                    hit.getZ() - sphere_center.getZ()
                );

                // Normalize normal
                double len = sqrt(Vec3::dot(normal, normal));
                normal.setX(normal.getX() / len);
                normal.setY(normal.getY() / len);
                normal.setZ(normal.getZ() / len);

                // Light direction (choose simple fixed light)
                Vec3 light_dir = Vec3(-1, 1, -1);

                // Normalize light
                double light_len = sqrt(Vec3::dot(light_dir, light_dir));
                light_dir.setX(light_dir.getX() / light_len);
                light_dir.setY(light_dir.getY() / light_len);
                light_dir.setZ(light_dir.getZ() / light_len);

                // Lambert
                double brightness = Vec3::dot(normal, light_dir);
                if (brightness < 0)
                    brightness = 0;

                int color = (int)(brightness * 255.0);

                *(unsigned int*)(data + offset) = (color << 16) | (color << 8) | color; // grayscale
                // *(unsigned int*)(data + offset) = 0x00FF0000; // Red
            }
            else {

                *(unsigned int*)(data + offset) = 0x000000FF; // Blue
            }
        }
    }
}

int main() {
    void* mlx = mlx_init();
    int     bits_per_pixel;
    int     size_line;
    int     endian;
    if (!mlx) {
        return 1;
    }

    void* win = mlx_new_window(mlx, IMG_WIDTH, IMG_HEIGHT, (char *)"MiniLibX Window");
    if (!win) {
        return 1;
    }
    void*  img = mlx_new_image(mlx, IMG_WIDTH, IMG_HEIGHT);
    void* data = mlx_get_data_addr(img, &bits_per_pixel, &size_line, &endian);

    render(data, size_line, bits_per_pixel);
    // 4. Put image to window

    mlx_hook(win, 17, 0, close_window, NULL);
    mlx_put_image_to_window(mlx, win, img, 0, 0);
    mlx_loop(mlx);

    return 0;
}
