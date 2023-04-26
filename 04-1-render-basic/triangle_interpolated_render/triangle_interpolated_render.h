#include "triangle_indexed_render.h"

#include <cmath>

struct vertex
{
    double x;
    double y;
    double z;

    double r;
    double g;
    double b;

    double u; // Texture coordinate u
    double v; // Texture coordinate v
};

double interpolate1kind(const double d1, const double d2, const double t);
double interpolate2kind(const double d1, const double d2, const double t);
vertex interpolate(const vertex& v1, const vertex& v2, const double t);

struct uniforms
{
    double  f0       = 0.;
    double  f1       = 0.;
    double  f2       = 0.;
    double  f3       = 0.;
    double  f4       = 0.;
    double  f5       = 0.;
    double  f6       = 0.;
    double  f7       = 0.;
    canvas* texture0 = nullptr;
};

struct gfx_program
{
    virtual ~gfx_program()                             = default;
    virtual void   set_uniforms(uniforms&)             = 0;
    virtual vertex vertex_shader(const vertex& v_in)   = 0;
    virtual rgb    fragment_shader(const vertex& v_in) = 0;
};

class triangle_interpolated_redner : public triangle_indexed_render
{
public:
    triangle_interpolated_redner(canvas& buffer);
    void set_gfx_program(gfx_program& program);
    void set_pen_color(rgb color);
    void draw_triangles(const std::vector<vertex>& vertexes,
                        const std::vector<uint>&   indexes);

private:
    std::vector<vertex> rasterize_triangle(const vertex& v1,
                                           const vertex& v2,
                                           const vertex& v3);
    std::vector<vertex> raster_horizontal_triangle(const vertex& single,
                                                   const vertex& left,
                                                   const vertex& right);

    void raster_one_horizontal_line(const vertex&        left_vertex,
                                    const vertex&        right_vertex,
                                    std::vector<vertex>& out);

    bool         pen_used = false;
    rgb          pen_color;
    gfx_program* program = nullptr;
};