#pragma once

#include "line_render.h"

class triangle_indexed_render : public line_render
{
public:
    triangle_indexed_render(canvas& buffer);

    pixels pixel_position_triangle(position& v1, position& v2, position& v3);
    void draw_triangles(pixels& vertex, std::vector<uint>& indexes, rgb color);
};