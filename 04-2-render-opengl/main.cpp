#include "game.h"
#include "sphere_engine.h"

#include <cstdlib>

int main()
{
    sphere_engine* my_engine = new sphere_engine();
    my_engine->initialize(config{});

    sphere_game my_game = sphere_game(my_engine);

    bool loop = true;

    event e;
    while (loop)
    {
        while (my_engine->event_keyboard(e))
        {
            if (e.action.quit)
            {
                loop = false;
                break;
            }
        }
        my_engine->render_triangle(triangle{
            vertex{ 0, 0, 0 }, vertex{ 0.1, 0.1, 0 }, vertex{ 0, 0.2, 0 } });

        my_engine->swap_buffers();
    }

    return EXIT_SUCCESS;
}