#include "game.h"
#include "objects/chessboard_cells.h"
#include "objects/sphere.h"

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>

int main()
{
    config cfg;
    cfg.shader_fragment_checker    = "./99-game/shaders/checker.frag";
    cfg.shader_fragment_chessboard = "./99-game/shaders/chessboard.frag";
    cfg.shader_fragment_chessboard_cells =
        "./99-game/shaders/chessboard_cells.frag";
    cfg.shader_vertex = "./99-game/shaders/shader.vert";
    cfg.texture_cells = "./99-game/textures/chessboard_texture1.png";

    game_checkers my_game;
    my_game.initialize(cfg);

    chessboard_cells obj_cells;
    my_game.add_figure(obj_cells);

    // sphere obj_sphere(2);
    // my_game.add_figure(obj_sphere);

    event e;
    float phi      = 0;
    auto time_last = std::chrono::steady_clock::now() - std::chrono::seconds(1);
    while (my_game.event_listener(e))
    {

        auto time_now = std::chrono::steady_clock::now();
        if ((time_now - time_last).count() > 1.e9 / fps)
        {
            // obj_cells.set_rotate(0, phi, 0);

            my_game.update();
            my_game.render();
            phi += 0.1;
        }
    }

    return EXIT_SUCCESS;
}