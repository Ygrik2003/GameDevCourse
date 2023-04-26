#include "triangle_indexed_render.h"

int main()
{
    int32_t width  = 320;
    int32_t height = 240;

    canvas image(width, height);

    triangle_indexed_render render(image);

    pixels vertexes;
    vertexes.push_back(position::generate(width, height));
    vertexes.push_back(position::generate(width, height));
    vertexes.push_back(position::generate(width, height));

    std::vector<uint> indexes;
    indexes.push_back(0);
    indexes.push_back(1);
    indexes.push_back(2);

    render.draw_triangles(vertexes, indexes, color_red);

    image.save_image("triangle.ppm");

    return EXIT_SUCCESS;
}