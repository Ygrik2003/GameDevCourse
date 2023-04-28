#include "canvas_basic.h"
#include "matrix.h"
#include "triangle_interpolated_render.h"
#include "view_convertation.h"

#include <algorithm>
#include <iostream>

constexpr double R      = 200;
constexpr double dTheta = 2 * M_PI / 5;
constexpr double dPhi   = M_PI / 5;

vertex get_sphere(double phi, double theta)
{

    return vertex{ R * (2 + cos(phi) * sin(theta)),
                   R * (2 + sin(phi) * sin(theta)),
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

    for (int thetaCounter = 0; thetaCounter <= M_PI / dTheta; thetaCounter++)
    {
        phi = 0;
        for (int phiCounter = 0; phiCounter <= 2 * M_PI / dPhi; phiCounter++)
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
    std::vector<double> arr{ vertexes[i1].z, vertexes[i2].z, vertexes[i3].z };
    return *std::max(arr.begin(), arr.end());
    // return (vertexes[i1].z + vertexes[i2].z + vertexes[i3].z) / 3;
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
            if (out.z > R || out.z < -R)
                std::cout << "ss: " << out.z << std::endl;
            matrix res = view.convert(static_cast<int>(out.x),
                                      static_cast<int>(out.y),
                                      static_cast<int>(out.z));
            out.x      = width * (res.getElement(0, 0) + 1) / 2;
            out.y      = height * (res.getElement(0, 1) + 1) / 2;
            out.z      = res.getElement(0, 2);

            // if (!(out.x > width || out.x < 0 || out.y > height || out.y < 0))
            std::cout << out.x << " " << out.y << std::endl;
            // out.x += R;
            // out.y += R;
            if ((out.x > width || out.x < 0 || out.y > height || out.y < 0))
            {
                out.x = 0;
                out.y = 0;
            }

            return out;
        }
        rgb fragment_shader(const vertex& v_in)
        {
            return { static_cast<uint8_t>(v_in.r),
                     static_cast<uint8_t>(v_in.g),
                     static_cast<uint8_t>(v_in.b) };
        }
        uniforms*         uniforms_1;
        view_convertation view =
            view_convertation(static_cast<double>(width) / height, -2 * R, R);
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

    image.save_image("04_interpolated_triangle.ppm");
    return EXIT_SUCCESS;
}