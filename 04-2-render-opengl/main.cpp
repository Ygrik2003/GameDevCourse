#include "game.h"
#include "objects/culinder.h"
#include "objects/sphere.h"
#include "sphere_engine.h"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

constexpr int fps = 30;

int main()
{
    engine* my_engine = new sphere_engine();
    my_engine->initialize(config{ "./04-2-render-opengl/shaders/shader.vert",
                                  "./04-2-render-opengl/shaders/shader.frag" });

    sphere_game my_game = sphere_game(my_engine);
    camera      _camera = camera(0.1, 30., M_PI / 2, 16. / 9.);

    sphere sphere_1(1);

    sphere_1.set_scale(1. / 2, 1. / 2, 1. / 2);
    sphere_1.set_translate(0, 2, 0);
    sphere_1.set_discretization(M_PI / 30, M_PI / 150);
    my_game.add_object(sphere_1);

    culinder culinder_1(1, 1);

    culinder_1.set_scale(1. / 2, 1. / 2, 1. / 2);
    culinder_1.set_rotate(0, 0, M_PI / 2);
    culinder_1.set_translate(0, 1, 0);
    culinder_1.set_discretization(M_PI / 10, 0.1);
    my_game.add_object(culinder_1);

    _camera.move(0, -2, 1);

    double phi;
    event  e;
    bool   loop = true;
    float  last_pos_x{};
    float  rotate_x = 0;
    float  rotate_y = 0;
    float  speed    = 0.05;
    auto time_last = std::chrono::steady_clock::now() - std::chrono::seconds(1);
    while (loop)
    {
        while (my_engine->event_keyboard(e))
        {
            if (e.action.quit)
            {
                loop = false;
                break;
            }
            else if (e.keyboard.up_clicked)
            {
                _camera.move(-speed * std::sin(rotate_x) * std::cos(rotate_y),
                             speed * std::sin(rotate_y),
                             -speed * std::cos(rotate_x) * std::cos(rotate_y));
            }
            else if (e.keyboard.down_clicked)
            {

                _camera.move(speed * std::sin(rotate_x) * std::cos(rotate_y),
                             -speed * std::sin(rotate_y),
                             speed * std::cos(rotate_x) * std::cos(rotate_y));
            }
            else if (e.keyboard.left_clicked)
            {
                _camera.move(
                    speed * std::cos(rotate_x), 0, -speed * std::sin(rotate_x));
            }
            else if (e.keyboard.right_clicked)
            {

                _camera.move(
                    -speed * std::cos(rotate_x), 0, speed * std::sin(rotate_x));
            }
            else if (e.motion.x || e.motion.y)
            {
                rotate_x += e.motion.x / 100;
                rotate_y += e.motion.y / 100;
                std::cout << rotate_x << " " << rotate_y << std::endl;
                _camera.set_rotate(0., rotate_x, rotate_y);
            }
            else if (e.keyboard.space_clicked)
            {
            }
        }
        auto time_now = std::chrono::steady_clock::now();
        if ((time_now - time_last).count() > 1.e9 / fps)
        {
            time_last = time_now;
            my_game.get_object(0).set_rotate(phi * 2, phi, phi / 2);

            for (auto it = my_game.begin(); it != my_game.end(); it++)
            {
                (*it).render(my_engine, _camera);
            }
            my_engine->swap_buffers();
            phi += (M_PI / 3) / fps;
        }
    }

    return EXIT_SUCCESS;
}