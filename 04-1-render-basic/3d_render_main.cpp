#include "canvas_basic.h"
#include "matrix.h"
#include "triangle_interpolated_render.h"
#include "view_convertation.h"

#include <SDL3/SDL.h>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>

constexpr int width  = 1000;
constexpr int height = 1000;

constexpr double r_big   = 100;
constexpr double r_small = 30;

constexpr double dTheta = M_PI / 15;
constexpr double dPhi   = 2 * M_PI / 15;

void vertexs_projection(const std::vector<vertex>& v_in,
                        std::vector<vertex>&       v_out,
                        view_convertation&         view)
{
    v_out.resize(v_in.size());
    for (int i = 0; i < v_in.size(); i++)
    {
        matrix res = view.convert(static_cast<int>(v_in[i].x),
                                  static_cast<int>(v_in[i].y),
                                  static_cast<int>(v_in[i].z));

        v_out[i].x = width * (1 + res.get_element(0, 0)) / 2;
        v_out[i].y = height * (1 + res.get_element(0, 1)) / 2;
        v_out[i].z = 1 + res.get_element(0, 2);

        v_out[i].r = v_in[i].r;
        v_out[i].g = v_in[i].g;
        v_out[i].b = v_in[i].b;
        v_out[i].u = v_in[i].u;
        v_out[i].v = v_in[i].v;
    }
}

struct program : gfx_program
{
    void   set_uniforms(uniforms& uniforms_) { uniforms_1 = &uniforms_; }
    vertex vertex_shader(const vertex& v_in)
    {
        vertex out(v_in);

        // if (std::pow(std::pow(uniforms_1->f0 - v_in.x, 2) +
        //                  std::pow(uniforms_1->f1 - v_in.y, 2),
        //              1. / 2.) > r_big)
        //     return out;

        // out.x += 100 * cos(uniforms_1->f0 - v_in.x);
        // out.y += 100 * cos(uniforms_1->f1 - v_in.y);

        return out;
    }
    rgb fragment_shader(const vertex& v_in)
    {
        double green =
            255 * std::exp(-std::pow(std::pow(uniforms_1->f0 - v_in.x, 2) +
                                         std::pow(uniforms_1->f1 - v_in.y, 2),
                                     1. / 2.) /
                           100);

        return { static_cast<uint8_t>(v_in.r),
                 static_cast<uint8_t>(green),
                 static_cast<uint8_t>(v_in.b) };
    }
    uniforms* uniforms_1;
} program_1;

vertex get_sphere(double phi, double theta, double r)
{

    return vertex{ r * (cos(phi) * sin(theta)),
                   r * (sin(phi) * sin(theta)),
                   r * (cos(theta)),
                   255 * std::pow(std::cos(theta), 2) };
}

void get_sphere_triangles(std::vector<vertex>& vertexes,
                          std::vector<uint>&   indexes,
                          double               r)
{
    auto get_index = [](int phi_c, int theta_c)
    {
        if (theta_c == 0)
        {
            return 0;
        }
        else if (theta_c == static_cast<int>(M_PI / dTheta) + 1)
        {
            std::cout << (static_cast<int>(M_PI / dTheta) + 1) *
                                 (static_cast<int>(2 * M_PI / dPhi)) +
                             1
                      << std::endl;
            return (static_cast<int>(M_PI / dTheta) + 1) *
                       (static_cast<int>(2 * M_PI / dPhi)) +
                   1;
        }
        else
        {
            return 1 + (theta_c - 1) * static_cast<int>(2 * M_PI / dPhi + 1) +
                   phi_c % static_cast<int>(2 * M_PI / dPhi);
        }
    };

    int    index = 0;
    double theta = 0, phi;

    vertexes.push_back(get_sphere(0, 0, r));
    for (int theta_counter = 1; theta_counter <= M_PI / dTheta; theta_counter++)
    {
        phi = 0;
        for (int phi_counter = 0; phi_counter <= 2 * M_PI / dPhi; phi_counter++)
        {
            vertexes.push_back(get_sphere(phi, theta, r));

            if (theta_counter < static_cast<int>(M_PI / dTheta))
            {
                indexes.push_back(get_index(phi_counter, theta_counter));
                indexes.push_back(get_index(phi_counter + 1, theta_counter));
                indexes.push_back(
                    get_index(phi_counter + 1, theta_counter + 1));

                indexes.push_back(get_index(phi_counter, theta_counter));
                indexes.push_back(get_index(phi_counter, theta_counter + 1));
                indexes.push_back(
                    get_index(phi_counter + 1, theta_counter + 1));
            }
            // else
            // {
            //     indexes.push_back(get_index(phi_counter, theta_counter));
            //     indexes.push_back(get_index(phi_counter + 1, theta_counter));
            //     indexes.push_back(
            //         get_index(phi_counter + 1, theta_counter + 1));
            // }
            phi += dPhi;
        }
        theta += dTheta;
    }
    vertexes.push_back(get_sphere(0, M_PI, r));
}

double mean_z(const std::vector<vertex>& vertexes, int i1, int i2, int i3)
{
    std::vector<double> arr{ vertexes[i1].z, vertexes[i2].z, vertexes[i3].z };
    return *std::min(arr.begin(), arr.end());
    // return (vertexes[i1].z + vertexes[i2].z + vertexes[i3].z) / 3;
}

int main()
{

    int    camera_x      = 0;
    int    camera_y      = 0;
    int    camera_z      = 0;
    double camera_rotate = 0;

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
    program_1.set_uniforms(uni);
    triangle_render.set_gfx_program(program_1);

    view_convertation view(static_cast<double>(width) / height, 1, 3 * r_big);

    double           phi        = 0;
    constexpr double d_phi      = M_PI / 30;
    bool             is_rotated = true;

    std::vector<vertex> vertexes;
    std::vector<vertex> current_vertexes;
    std::vector<uint>   indexes;
    std::vector<uint>   current_indexes;

    get_sphere_triangles(vertexes, indexes, r_big);

    double mouse_x = 0, mouse_y = 0;
    bool   is_alive = true;
    while (is_alive)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                is_alive = false;
                break;
            }
            else if (event.type == SDL_EVENT_MOUSE_MOTION)
            {
                uni.f0 = event.motion.x;
                uni.f1 = event.motion.y;
            }
            else if (event.type == SDL_EVENT_KEY_DOWN)
            {
                if (event.key.keysym.sym == SDLK_w)
                    camera_z -= 10;
                if (event.key.keysym.sym == SDLK_s)
                    camera_z += 10;
                if (event.key.keysym.sym == SDLK_a)
                    camera_x -= 10;
                if (event.key.keysym.sym == SDLK_d)
                    camera_x += 10;

                if (event.key.keysym.sym == SDLK_RIGHT)
                    camera_rotate -= 0.1;
                if (event.key.keysym.sym == SDLK_LEFT)
                    camera_rotate += 0.1;

                if (event.key.keysym.sym == SDLK_SPACE)
                    is_rotated = !is_rotated;
            }
        }

        view.set_translation(camera_x, camera_y, camera_z + 10 * r_big);
        view.set_rotation(0, camera_rotate, 0);
        ;

        vertexs_projection(vertexes, current_vertexes, view);
        current_indexes = indexes;

        for (int i = 0; i < current_indexes.size(); i += 3)
        {
            for (int j = i + 3; j < current_indexes.size(); j += 3)
            {
                if (mean_z(current_vertexes,
                           current_indexes[i + 0],
                           current_indexes[i + 1],
                           current_indexes[i + 2]) <
                    mean_z(current_vertexes,
                           current_indexes[j + 0],
                           current_indexes[j + 1],
                           current_indexes[j + 2]))
                {
                    std::swap(current_indexes[i + 0], current_indexes[j + 0]);
                    std::swap(current_indexes[i + 1], current_indexes[j + 1]);
                    std::swap(current_indexes[i + 2], current_indexes[j + 2]);
                }
            }
        }

        triangle_render.clear({ 255, 255, 255 });
        triangle_render.draw_triangles(current_vertexes, current_indexes);

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

        // std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));

        if (is_rotated)
            phi += d_phi;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window_main);

    SDL_Quit();

    return EXIT_SUCCESS;
}