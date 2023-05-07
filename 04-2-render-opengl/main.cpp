#include "game.h"
#include "sphere_engine.h"

#include <chrono>
#include <cstdlib>
#include <thread>

int main()
{
    engine* my_engine = new sphere_engine();
    my_engine->initialize(config{ "./04-2-render-opengl/shaders/shader.vert",
                                  "./04-2-render-opengl/shaders/shader.frag" });

    sphere_game my_game = sphere_game(my_engine);

    sphere sphere_1(1);
    sphere_1.set_discretization(M_PI / 10, M_PI / 10);
    // sphere_1.set_scale(9. / 16., 1., 1.);
    my_game.add_object(sphere_1);

    double phi;
    event  e;
    bool   loop = true;
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
            }
        }
        my_game.get_object(0).set_rotate(0.f, phi, 0.f);

        for (auto it = my_game.begin(); it != my_game.end(); it++)
        {
            (*it).render(my_engine);
        }
        my_engine->swap_buffers();

        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));
        phi += M_PI / 400;
    }

    return EXIT_SUCCESS;
}