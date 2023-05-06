#include "game.h"
#include "sphere_engine.h"

#include <cstdlib>

int main()
{
    engine* my_engine = new sphere_engine();
    my_engine->initialize(config{});

    sphere_game my_game = sphere_game(my_engine);

    bool loop = true;

    sphere sphere_1(1);
    my_game.add_object(sphere_1);

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
        for (auto it = my_game.begin(); it != my_game.end(); it++)
        {
            (*it).render(my_engine);
        }
        my_engine->swap_buffers();
    }

    return EXIT_SUCCESS;
}