#include "triangle_interpolated_render.h"

#include <algorithm>
#include <array>

double interpolate1kind(double x1, double x2, double t)
{
    if (t > 1 || t < 0)
        throw std::runtime_error("Interpolation1kind out of range");
    return (x2 - x1) * t + x1;
}

double interpolate2kind(const double x1, const double x2, const double t)
{
    if (t > 1 || t < 0)
        throw std::runtime_error("Interpolation2kind out of range");
    return (x2 - x1) * std::pow(t, 2) + x1;
}

double interpolate_sin(const double x1, const double x2, const double t)
{
    if (t > 1 || t < 0)
        throw std::runtime_error("Interpolation2kind out of range");
    return (x2 - x1) * std::sin(M_PI * t) + x1;
}

vertex interpolate(const vertex& v1, const vertex& v2, const double t)
{
    return { interpolate1kind(v1.x, v2.x, t), interpolate1kind(v1.y, v2.y, t),
             interpolate1kind(v1.z, v2.z, t), interpolate1kind(v1.r, v2.r, t),
             interpolate1kind(v1.g, v2.g, t), interpolate1kind(v1.b, v2.b, t),
             interpolate1kind(v1.u, v2.u, t), interpolate1kind(v1.v, v2.v, t) };
}

triangle_interpolated_redner::triangle_interpolated_redner(canvas& buffer)
    : triangle_indexed_render(buffer)
{
}

void triangle_interpolated_redner::set_gfx_program(gfx_program& program)
{
    this->program = &program;
}

void triangle_interpolated_redner::set_pen_color(rgb color)
{
    pen_used  = true;
    pen_color = color;
}

void triangle_interpolated_redner::draw_triangles(
    const std::vector<vertex>& vertexes, const std::vector<uint>& indexes)
{
    for (int i = 0; i < indexes.size(); i += 3)
    {
        const uint32_t i1 = indexes[i + 0];
        const uint32_t i2 = indexes[i + 1];
        const uint32_t i3 = indexes[i + 2];

        const vertex v1 = vertexes.at(i1);
        const vertex v2 = vertexes.at(i2);
        const vertex v3 = vertexes.at(i3);

        const vertex v1_ = program->vertex_shader(v1);
        const vertex v2_ = program->vertex_shader(v2);
        const vertex v3_ = program->vertex_shader(v3);

        if (v1_.x < -200 || v2_.x < -200 || v3_.x < -200 || v1_.y < -200 ||
            v2_.y < -200 || v3_.y < -200 || v1_.x > 1200 || v2_.x > 1200 ||
            v3_.x > 1200 || v1_.y > 1200 || v2_.y > 1200 || v3_.y > 1200)
            return;

        std::vector<vertex> triangle = rasterize_triangle(v1_, v2_, v3_);

        for (auto pixel : triangle)
        {
            rgb color = program->fragment_shader(pixel);

            set_pixel(
                {
                    static_cast<int32_t>(std::round(pixel.x)),
                    static_cast<int32_t>(std::round(pixel.y)),
                },
                pixel.z,
                color);
        }
        if (pen_used)
        {
            position pos1{ static_cast<int>(v1_.x), static_cast<int>(v1_.y) };
            position pos2{ static_cast<int>(v2_.x), static_cast<int>(v2_.y) };
            position pos3{ static_cast<int>(v3_.x), static_cast<int>(v3_.y) };
            draw_line(pos1, pos2, pen_color);
            draw_line(pos2, pos3, pen_color);
            draw_line(pos3, pos1, pen_color);
        }
    }
}

std::vector<vertex> triangle_interpolated_redner::rasterize_triangle(
    const vertex& v1, const vertex& v2, const vertex& v3)
{
    std::vector<vertex> out;

    std::array<const vertex*, 3> vertexes{ &v1, &v2, &v3 };
    std::sort(vertexes.begin(),
              vertexes.end(),
              [](const vertex* v1, const vertex* v2) { return v1->y < v2->y; });

    const vertex& vertex_top = *vertexes[0];
    const vertex& vertex_mid = *vertexes[1];
    const vertex& vertex_bot = *vertexes[2];

    position top{ static_cast<int32_t>(vertex_top.x),
                  static_cast<int32_t>(vertex_top.y) };
    position mid{ static_cast<int32_t>(vertex_mid.x),
                  static_cast<int32_t>(vertex_mid.y) };
    position bot{ static_cast<int32_t>(vertex_bot.x),
                  static_cast<int32_t>(vertex_bot.y) };

    for (auto [v1, v2, v3] :
         { std::array{ vertex_top, vertex_mid, vertex_bot },
           std::array{ vertex_top, vertex_bot, vertex_mid },
           std::array{ vertex_mid, vertex_bot, vertex_top } })
    {
        position pos1{ static_cast<int32_t>(v1.x), static_cast<int32_t>(v1.y) };
        position pos2{ static_cast<int32_t>(v2.x), static_cast<int32_t>(v2.y) };
        position pos3{ static_cast<int32_t>(v3.x), static_cast<int32_t>(v3.y) };
        if (pos1 == pos2)
        {
            pixels line = pixels_positions(pos1, pos3);

            for (int i = 0; i < line.size(); i++)
            {
                // TODO
                vertex v = interpolate(
                    v1, v3, static_cast<double>(i) / (line.size() + 1));
                v.x = line[i].x;
                v.y = line[i].y;
                out.push_back(v);
            }
            return out;
        }
        if (pos1.y == pos2.y)
        {
            std::vector<vertex> triangle =
                raster_horizontal_triangle(v3, v1, v2);

            out.insert(out.end(), triangle.begin(), triangle.end());
            return out;
        }
    }
    pixels longest_side_line = pixels_positions(top, bot);

    position second_mid = *std::find_if(
        begin(longest_side_line),
        std::end(longest_side_line),
        [&](const position& pos)
        { return pos.y == static_cast<int32_t>(std::round(vertex_mid.y)); });

    double t         = 0;
    double end_start = (bot - top).length();
    if (end_start != 0)
    {
        t = (second_mid - top).length() / end_start;
    }

    vertex vertex_second_mid = interpolate(vertex_top, vertex_bot, t);

    std::vector<vertex> top_triangle =
        raster_horizontal_triangle(vertex_top, vertex_mid, vertex_second_mid);

    std::vector<vertex> bot_triangle =
        raster_horizontal_triangle(vertex_bot, vertex_mid, vertex_second_mid);

    out.insert(out.end(), top_triangle.begin(), top_triangle.end());
    out.insert(out.end(), bot_triangle.begin(), bot_triangle.end());

    return out;
}

std::vector<vertex> triangle_interpolated_redner::raster_horizontal_triangle(
    const vertex& single, const vertex& left, const vertex& right)
{
    std::vector<vertex> out;

    uint32_t lines_count = std::ceil(std::abs(single.y - left.y)) + 1;

    for (int i = 0; i <= lines_count; i++)
    {
        double t            = static_cast<double>(i) / (lines_count);
        vertex left_vertex  = interpolate(single, left, t);
        vertex right_vertex = interpolate(single, right, t);

        raster_one_horizontal_line(left_vertex, right_vertex, out);
    }

    return out;
}

void triangle_interpolated_redner::raster_one_horizontal_line(
    const vertex&        left_vertex,
    const vertex&        right_vertex,
    std::vector<vertex>& out)
{
    uint32_t pixels_count =
        std::ceil(std::abs(left_vertex.x - right_vertex.x)) + 1;

    if (pixels_count > 0)
    {
        for (int i = 0; i <= pixels_count; i++)
        {
            out.push_back(interpolate(left_vertex,
                                      right_vertex,
                                      static_cast<double>(i) / (pixels_count)));
        }
    }
    else
    {
        out.push_back(left_vertex);
    }
}