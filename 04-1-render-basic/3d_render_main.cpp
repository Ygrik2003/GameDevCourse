#include "canvas_basic.h"
#include "matrix.h"
#include "triangle_interpolated_render.h"
#include "view_convertation.h"

#include <SDL3/SDL.h>
#include <algorithm>
#include <chrono>
#include <iostream>

constexpr int width  = 1000;
constexpr int height = 1000;

constexpr double R      = 100;
constexpr double dTheta = 2 * M_PI / 50;
constexpr double dPhi   = M_PI / 15;

struct program : gfx_program
{
    void   set_uniforms(uniforms& uniforms_) { this->uniforms_1 = &uniforms_; }
    vertex vertex_shader(const vertex& v_in)
    {
        vertex out(v_in);

        matrix res = view.convert(static_cast<int>(out.x),
                                  static_cast<int>(out.y),
                                  static_cast<int>(out.z));

        out.x = width * (1 + res.getElement(0, 0)) / 2;
        out.y = height * (1 + res.getElement(0, 1)) / 2;
        out.z = 1 + res.getElement(0, 2);

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
        view_convertation(static_cast<double>(width) / height, 1, 3 * R);
} program_1;

vertex get_sphere(double phi, double theta)
{

    return vertex{ R * (cos(phi) * sin(theta)),
                   R * (sin(phi) * sin(theta)),
                   R * (2 + cos(theta)),
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
    if (0 != SDL_Init(SDL_INIT_VIDEO))
    {
        std::cout << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    SDL_Window* window_main =
        SDL_CreateWindow("3D Render", width, height, SDL_WINDOW_OPENGL);

    if (window_main == nullptr)
    {
        std::cout << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    SDL_Renderer* renderer =
        SDL_CreateRenderer(window_main, nullptr, SDL_RENDERER_ACCELERATED);

    if (renderer == nullptr)
    {
        std::cout << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    canvas    image(width, height);
    void*     pixels = image.get_pixels().data();
    const int depth  = sizeof(rgb) * 8;
    const int pitch  = width * sizeof(rgb);

    triangle_interpolated_redner triangle_render(image);
    triangle_render.set_pen_color(rgb{ 0, 0, 0 });
    triangle_render.clear(rgb{ 255, 255, 255 });

    uniforms uni;
    double   phi = 0;
    program_1.set_uniforms(uni);
    triangle_render.set_gfx_program(program_1);

    std::vector<vertex> vertexes;
    std::vector<uint>   indexes;

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

    get_sphere_triangles(vertexes, indexes);

    bool                                       is_alive = true;
    std::chrono::_V2::steady_clock::time_point last_time;
    while (is_alive)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                is_alive = false;
            break;
        }
        auto curr_time = std::chrono::steady_clock::now();
        if (static_cast<std::chrono::duration<double>>(curr_time - last_time)
                .count() < 1. / 60.)
            continue;
        last_time = curr_time;

        program_1.view.set_translation(0, 0, 5 * R);
        // program_1.view.set_translation(R * cos(phi), R * sin(phi), 50 * R);
        program_1.view.set_rotation(0, 0, phi);
        phi += dPhi / 5;

        triangle_render.clear({ 255, 255, 255 });
        triangle_render.draw_triangles(vertexes, indexes);

        SDL_Surface* bitmapSurface = SDL_CreateSurfaceFrom(
            pixels, width, height, pitch, SDL_PIXELFORMAT_RGB24);

        if (bitmapSurface == nullptr)
        {
            std::cout << SDL_GetError() << std::endl;
            return EXIT_FAILURE;
        }

        SDL_Texture* bitmapTex =
            SDL_CreateTextureFromSurface(renderer, bitmapSurface);

        if (bitmapTex == nullptr)
        {
            std::cout << SDL_GetError() << std::endl;
            return EXIT_FAILURE;
        }

        SDL_DestroySurface(bitmapSurface);

        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, bitmapTex, nullptr, nullptr);
        SDL_RenderPresent(renderer);

        SDL_DestroyTexture(bitmapTex);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window_main);

    SDL_Quit();

    return EXIT_SUCCESS;
}