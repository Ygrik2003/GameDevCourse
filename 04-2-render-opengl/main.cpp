#include "game.h"
#include "sphere_engine.h"

#include <cstdlib>

int main()
{
    sphere_engine my_engine = sphere_engine();
    my_engine.initialize(config{});

    sphere_game my_game = sphere_game(my_engine);

    bool loop = true;

    event e;
    while (loop)
    {
        while (my_engine.event_keyboard(e))
        {
            if (e.action.quit)
            {
                loop = false;
                break;
            }
        }
    }

    return EXIT_SUCCESS;
}