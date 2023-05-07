#include "game.h"
#include "sphere_engine.h"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

int main()
{
    engine* my_engine = new sphere_engine();
    my_engine->initialize(config{ "./04-2-render-opengl/shaders/shader.vert",
                                  "./04-2-render-opengl/shaders/shader.frag" });

    sphere_game my_game = sphere_game(my_engine);
    camera      _camera = camera(1., 30., M_PI / 2, 16. / 9.);

    sphere sphere_1(1);

    sphere_1.set_scale(1. / 2, 1. / 2, 1. / 2);
    sphere_1.set_translate(0, 0, 1);
    sphere_1.set_discretization(M_PI / 10, M_PI / 10);
    my_game.add_object(sphere_1);

    sphere sphere_2(1);

    sphere_2.set_scale(1. / 2, 1. / 2, 1. / 2);
    sphere_2.set_translate(0, 0, -1);
    sphere_2.set_discretization(M_PI / 10, M_PI / 10);
    my_game.add_object(sphere_2);

    double phi;
    event  e;
    bool   loop = true;
    float  last_pos_x{};
    float  rotate_x = 0;
    float  rotate_y = 0;
    float  speed    = 0.05;
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
                _camera.move(
                    speed * std::sin(rotate_x), 0, -speed * std::cos(rotate_x));
            }
            else if (e.keyboard.down_clicked)
            {
                _camera.move(
                    -speed * std::sin(rotate_x), 0, speed * std::cos(rotate_x));
            }
            else if (e.keyboard.left_clicked)
            {
                _camera.move(
                    speed * std::cos(rotate_x), 0, speed * std::sin(rotate_x));
            }
            else if (e.keyboard.right_clicked)
            {

                _camera.move(-speed * std::cos(rotate_x),
                             0,
                             -speed * std::sin(rotate_x));
            }
            else if (e.motion.x || e.motion.y)
            {
                rotate_x += e.motion.x / 200;
                rotate_y -= e.motion.y / 200;
                _camera.set_rotate(0., -rotate_x, rotate_y);
            }
        }
        // my_game.get_object(0).set_rotate(0.f, phi, 0.f);

        for (auto it = my_game.begin(); it != my_game.end(); it++)
        {
            (*it).render(my_engine, _camera);
        }
        my_engine->swap_buffers();

        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));
        phi += M_PI / 400;
    }

    return EXIT_SUCCESS;
}