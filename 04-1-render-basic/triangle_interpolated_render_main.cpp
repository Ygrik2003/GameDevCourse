#include "canvas_basic.h"
#include "triangle_interpolated_render.h"
#include "vector3d.h"

#include <algorithm>

constexpr double pi = 3.1415926535;

constexpr double R      = 250;
constexpr double dTheta = 0.1;
constexpr double dPhi   = 0.1;

const vector3d camera_pos(100., 100., -100.);

vertex get_sphere(double phi, double theta)
{

    return vertex{ R * cos(phi) * sin(theta),
                   R * sin(phi) * sin(theta),
                   R * cos(theta),
                   255 * std::pow(std::cos(theta), 2),
                   0,
                   0,
                   0,
                   0 };
}

void get_sphere_triangles(std::vector<vertex>& vertexes,
                          std::vector<uint>&   indexes)
{

    int index = 0;

    double theta = 0, phi;

    for (int thetaCounter = 0; thetaCounter <= pi / dTheta; thetaCounter++)
    {
        phi = 0;
        for (int phiCounter = 0; phiCounter <= 2 * pi / dPhi; phiCounter++)
        {
            vertexes.push_back(get_sphere(phi, theta));
            vertexes.push_back(get_sphere(phi, theta + dTheta));
            vertexes.push_back(get_sphere(phi + dPhi, theta));
            vertexes.push_back(get_sphere(phi + dPhi, theta + dTheta));

            indexes.push_back(vertexes.size() - 4);
            indexes.push_back(vertexes.size() - 3);
            indexes.push_back(vertexes.size() - 2);
            indexes.push_back(vertexes.size() - 3);
            indexes.push_back(vertexes.size() - 2);
            indexes.push_back(vertexes.size() - 1);

            phi += dPhi;
        }
        theta += dTheta;
    }
}

double mean_z(const std::vector<vertex>& vertexes, int i1, int i2, int i3)
{
    return (vertexes[i1].z + vertexes[i2].z + vertexes[i3].z) / 3;
}

int main()
{
    constexpr int width  = 1000;
    constexpr int height = 1000;

    canvas image(width, height);

    triangle_interpolated_redner render(image);
    // render.set_pen_color(rgb{ 0, 0, 0 });
    render.clear(rgb{ 255, 255, 255 });

    struct program : gfx_program
    {
        void set_uniforms(uniforms& uniforms_)
        {
            this->uniforms_1 = &uniforms_;
        }
        vertex vertex_shader(const vertex& v_in)
        {
            vertex out(v_in);

            // constexpr double angle = 3.1415 * 0;

            double x = out.x /* + std::sqrt(2) * out.z*/;
            double y = out.y /* + std::sqrt(2) * out.z*/;

            // out.x = x * std::cos(angle) - y * std::sin(angle);
            // out.y = x * std::sin(angle) + y * std::cos(angle);

            out.x = x + 2 * R;
            out.y = y + 2 * R;

            return out;
        }
        rgb fragment_shader(const vertex& v_in)
        {
            return { static_cast<uint8_t>(v_in.r),
                     static_cast<uint8_t>(v_in.g),
                     static_cast<uint8_t>(v_in.b) };
        }
        uniforms* uniforms_1;
    } program_1;

    uniforms uni;

    program_1.set_uniforms(uni);
    render.set_gfx_program(program_1);

    std::vector<vertex> vertexes;
    std::vector<uint>   indexes;

    get_sphere_triangles(vertexes, indexes);

    for (int i = 0; i < indexes.size(); i += 3)
    {
        for (int j = i + 3; j < indexes.size(); j += 3)
        {
            if (mean_z(
                    vertexes, indexes[i + 0], indexes[i + 1], indexes[i + 2]) >
                mean_z(
                    vertexes, indexes[j + 0], indexes[j + 1], indexes[j + 2]))
            {
                std::swap(indexes[i + 0], indexes[j + 0]);
                std::swap(indexes[i + 1], indexes[j + 1]);
                std::swap(indexes[i + 2], indexes[j + 2]);
            }
        }
    }

    render.draw_triangles(vertexes, indexes);

    // render.draw_triangles(
    //     {
    //         { 0, 0, 0, 255, 0, 0, 0, 0 },
    //         { 100, 100, 0, 0, 255, 0, 0, 0 },
    //         { 100, 0, 0, 0, 0, 255, 0, 0 },
    //         { 110, 0, 0, 255, 0, 0, 0, 0 },
    //         { 210, 100, 0, 0, 255, 0, 0, 0 },
    //         { 210, 10, 0, 0, 0, 255, 0, 0 },
    //     },
    //     { 0, 1, 2, 3, 4, 5 });

    image.save_image("04_interpolated_triangle.ppm");
    return EXIT_SUCCESS;
}