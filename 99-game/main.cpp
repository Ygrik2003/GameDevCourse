#include "engine/engine.h"
#include "game.h"

#include <chrono>
#include <cmath>
#include <cstdlib>

constexpr uint32_t fps = 60;

int main()
{
    config cfg;
    cfg.shader_fragment_checker    = "./99-game/shaders/checker.frag";
    cfg.shader_fragment_chessboard = "./99-game/shaders/chessboard.frag";
    cfg.shader_fragment_chessboard_cells =
        "./99-game/shaders/chessboard_cells.frag";
    cfg.shader_vertex = "./99-game/shaders/shader.vert";
    cfg.texture_cells = "./99-game/textures/chessboard_texture.png";

    uniform uniforms;
    uniforms.width  = cfg.width;
    uniforms.height = cfg.height;

    engine_checker* my_engine = new engine_checker();
    my_engine->set_uniform(uniforms);
    my_engine->initialize(cfg);

    event e;
    bool  loop     = true;
    float rotate_x = 0;
    float rotate_y = 0;
    float speed    = 0.05;
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
            }
            else if (e.keyboard.down_clicked)
            {
            }
            else if (e.keyboard.left_clicked)
            {
            }
            else if (e.keyboard.right_clicked)
            {
            }
            else if (e.motion.x || e.motion.y)
            {
            }
            else if (e.keyboard.space_clicked)
            {
            }
        }
        auto time_now = std::chrono::steady_clock::now();
        if ((time_now - time_last).count() > 1.e9 / fps)
        {
            my_engine->render_triangle(
                triangle(vertex_textured(vertex(-1, -1, 0), 0, 0),
                         vertex_textured(vertex(-1, 1, 0), 0, 8),
                         vertex_textured(vertex(1, -1, 0), 8, 0)));
            my_engine->render_triangle(
                triangle(vertex_textured(vertex(1, 1, 0), 8, 8),
                         vertex_textured(vertex(-1, 1, 0), 0, 8),
                         vertex_textured(vertex(1, -1, 0), 8, 0)));
            my_engine->swap_buffers();
        }
    }

    return EXIT_SUCCESS;
}