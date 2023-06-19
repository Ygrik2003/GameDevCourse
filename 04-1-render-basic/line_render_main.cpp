#include "line_render.h"

int main(int, char**)
{
    const rgb black = { 0, 0, 0 };
    const rgb white = { 255, 255, 255 };
    const rgb green = { 0, 255, 0 };

    int32_t width  = 320;
    int32_t height = 240;

    canvas image(width, height);

    line_render render(image);

    render.clear(black);

    for (size_t i = 0; i < 100; ++i)
    {
        position start = position::generate(width, height);
        position end   = position::generate(width, height);
        rgb      color = rgb::generate();

        render.draw_line(start, end, color);
    }

    image.save_image("01_lines.ppm");
    return 0;
}
