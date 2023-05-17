#include "game.h"
#include "objects/chessboard.h"
#include "objects/chessboard_cells.h"

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>

int main()
{
    config cfg;
    cfg.shader_fragment    = "./99-game/shaders/shader.frag";
    cfg.shader_vertex      = "./99-game/shaders/shader.vert";
    cfg.texture_cells      = "./99-game/textures/texture_cells.png";
    cfg.texture_checker    = "./99-game/textures/texture_cells.png";
    cfg.texture_chessboard = "./99-game/textures/texture_chessboard.png";

    game_checkers my_game;
    my_game.initialize(cfg);

    chessboard_cells obj_cells;
    my_game.add_figure(obj_cells, cfg.texture_cells, 0);

    chessboard obj_chessboard;
    my_game.add_figure(obj_chessboard, cfg.texture_cells, 1);

    event e;
    float phi      = 0;
    auto time_last = std::chrono::steady_clock::now() - std::chrono::seconds(1);
    while (my_game.event_listener(e))
    {

        auto time_now = std::chrono::steady_clock::now();
        if ((time_now - time_last).count() > 1.e9 / fps)
        {
            my_game.update();
            my_game.render();
        }
    }

    return EXIT_SUCCESS;
}