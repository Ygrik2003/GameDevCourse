#include "game.h"
#include "objects/model.h"

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>

int main()
{
    config cfg;
    cfg.shader_fragment = "./99-game/shaders/shader.frag";
    cfg.shader_vertex   = "./99-game/shaders/shader.vert";
    cfg.texture_block   = "./99-game/textures/texture_block.png";
    cfg.texture_board   = "./99-game/textures/texture_board.png";

    game_tetris my_game;
    if (my_game.initialize(cfg))
    {
        return -1;
    }

    model  model_board("./99-game/textures/board.obj");
    figure obj_board = model_board.get_meshes()[0];

    my_game.add_figure(obj_board, cfg.texture_board, 1);

    model model_cube("./99-game/textures/primitive2x2x2.obj");

    figure figure_temp;

    // cube 2x2x2
    figure primitive_figure_1;
    for (int i = 0; i < model_cube.get_meshes().size(); i++)
    {
        figure_temp = model_cube.get_meshes()[i];
        primitive_figure_1.add_figure(figure_temp);
    }
    primitive_figure_1.set_scale(0.1, 0.1, 0.1);
    primitive_figure_1.set_translate(0, 1, 0);
    my_game.add_figure(primitive_figure_1, cfg.texture_block, 0);

    event e{};
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