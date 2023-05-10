#include "triangle_indexed_render.h"
#include <utility>

triangle_indexed_render::triangle_indexed_render(canvas& buffer)
    : line_render(buffer)
{
}

pixels triangle_indexed_render::pixel_position_triangle(position& v1,
                                                        position& v2,
                                                        position& v3)
{
    pixels pixel_pos;
    for (auto [start, end] :
         { std::pair{ v1, v2 }, std::pair{ v2, v3 }, std::pair{ v3, v1 } })
    {
        pixels p = pixels_positions(start, end);
        pixel_pos.insert(pixel_pos.end(), p.begin(), p.end());
    }
    return pixel_pos;
}

void triangle_indexed_render::draw_triangles(pixels&            vertexes,
                                             std::vector<uint>& indexes,
                                             rgb                color)
{
    pixels triangle_pixels;

    for (int i = 0; i < indexes.size(); i += 3)
    {
        uint32_t p1 = indexes[i + 0];
        uint32_t p2 = indexes[i + 1];
        uint32_t p3 = indexes[i + 2];

        position vertex1 = vertexes.at(p1);
        position vertex2 = vertexes.at(p2);
        position vertex3 = vertexes.at(p3);

        pixels lines_pixels =
            pixel_position_triangle(vertex1, vertex2, vertex3);

        triangle_pixels.insert(
            triangle_pixels.end(), lines_pixels.begin(), lines_pixels.end());

        for (auto pixel : triangle_pixels)
        {
            set_pixel(pixel, 0, color);
        }
    }
}