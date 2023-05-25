#include "game.h"
#include "objects/model.h"

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>

int main()
{
    config cfg;
    cfg.shader_fragment       = "./99-game/shaders/shader.frag";
    cfg.shader_vertex         = "./99-game/shaders/shader.vert";
    cfg.texture_cells         = "./99-game/textures/texture_cells.png";
    cfg.texture_checker_white = "./99-game/textures/texture_checker_white.png";
    cfg.texture_checker_black = "./99-game/textures/texture_checker_black.png";
    cfg.texture_board         = "./99-game/textures/texture_board.png";

    game_tetris my_game;
    my_game.initialize(cfg);

    model  model_board("./99-game/textures/board.obj");
    figure obj_board = model_board.get_meshes()[0];
    my_game.add_figure(obj_board, cfg.texture_cells, 1);

    model model_cube("./99-game/textures/cube.obj");
    for (int i = 0; i < 100; i++)
    {
        figure obj_cube = model_cube.get_meshes()[0];
        obj_cube.set_scale(0.1, 0.1, 0.1);
        obj_cube.set_translate(1, 1 + (0.2 + 0.001) * i, 0);
        my_game.add_figure(obj_cube, cfg.texture_cells, 0);
    }

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