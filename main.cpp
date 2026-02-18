
// #include "includes/Constants.hpp"
// #include "includes/Material.hpp"
// #include "includes/Camera.hpp"
extern "C" {
    #include "minilibx-linux/mlx.h"
}
# include <stdlib.h>

int    close_window()
{
    exit(9);
    return 0;
}


void render(void* data, int size_line, int bits_per_pixel)
{
    int x = 100;
    int y = 100;

    int bytes_per_pixel = bits_per_pixel / 8;
    int offset = (y * size_line) + (x * bytes_per_pixel);

    for (int x=0, x < 100, x++)
    {
        for (int y = )
    }
    *(unsigned int*)(data + offset) = 0x00FF0000;
}

int main() {
    void* mlx = mlx_init();
    int     bits_per_pixel;
    int     size_line;
    int     endian;
    if (!mlx) {
        return 1;
    }
    const int image_width = 800;
    const int image_height = 600;

    void* win = mlx_new_window(mlx, image_width, image_height, (char *)"MiniLibX Window");
    if (!win) {
        return 1;
    }
    void*  img = mlx_new_image(mlx, image_width, image_height);
    void* data = mlx_get_data_addr(img, &bits_per_pixel, &size_line, &endian);

    render(data, size_line, bits_per_pixel);
    // 4. Put image to window

    mlx_hook(win, 17, 0, close_window, NULL);
    mlx_put_image_to_window(mlx, win, img, 0, 0);
    mlx_loop(mlx);

    return 0;
}
