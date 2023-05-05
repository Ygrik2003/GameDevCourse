#pragma once

#include "irender.h"

struct line_render : irender
{
public:
    line_render(canvas& buffer_);

    void clear(rgb) override;

    void   set_pixel(position, double z, rgb) override;
    pixels pixels_positions(position start, position end) override;
    void   draw_line(position start, position end, rgb);

private:
    canvas& buffer;
};