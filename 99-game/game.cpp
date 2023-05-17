#include "game.h"

int game_checkers::initialize(config cfg)
{
    this->cfg = cfg;

    uniforms;
    uniforms.width  = cfg.width;
    uniforms.height = cfg.height;

    cam = camera();
    cam.uniform_link(uniforms);
    cam.move(0, 1, 0);
    cam.set_rotate(0, M_PI / 2, -3 * M_PI / 2);

    my_engine = new engine_checkers();
    my_engine->set_uniform(uniforms);
    my_engine->initialize(cfg);

    my_engine->load_texture(0, cfg.texture_cells);
    my_engine->load_texture(1, cfg.texture_chessboard);
    my_engine->load_texture(2, cfg.texture_checker);

    return 1;
};

bool game_checkers::event_listener(event& e)
{
    while (my_engine->event_keyboard(e))
    {
        if (e.action.quit)
        {
            return false;
        }
        // else if (e.keyboard.up_clicked)
        // {
        //     cam.move_forward(cfg.camera_speed);
        // }
        // else if (e.keyboard.down_clicked)
        // {
        //     cam.move_backward(cfg.camera_speed);
        // }
        // else if (e.keyboard.left_clicked)
        // {
        //     cam.move_left(cfg.camera_speed);
        // }
        // else if (e.keyboard.right_clicked)
        // {

        //     cam.move_right(cfg.camera_speed);
        // }
        else if (e.motion.x || e.motion.y)
        {
            // cam.add_rotate(0, cfg.camera_speed_rotate * e.motion.x, 0);
            cam.add_rotate(0,
                           cfg.camera_speed_rotate * e.motion.x,
                           cfg.camera_speed_rotate * e.motion.y);
        }
        else if (e.keyboard.space_clicked)
        {
            my_engine->reload_shader(cfg.shader_vertex, GL_VERTEX_SHADER);
            my_engine->reload_shader(cfg.shader_fragment, GL_FRAGMENT_SHADER);
        }
    }
    return true;
};

void game_checkers::update()
{
    phi += M_PI / 300;
    theta += M_PI / 300;

    cam.add_rotate(0, -M_PI / 300, 0);
    cam.set_translate(1 * std::cos(phi), -1, 1 * std::sin(phi));
};

void game_checkers::render()
{

    for (auto fig : figures)
    {
        fig->uniform_link(uniforms);
        // fig->set_rotate(0, 0, theta);
        my_engine->set_texture(fig->get_texture_index());
        for (size_t i = 0; i < fig->get_count(); i++)
        {
            triangle tr = fig->get_triangle(i);
            my_engine->render_triangle(tr);
        }
    }
    my_engine->swap_buffers();
};

void game_checkers::add_figure(figure&     fig,
                               const char* texture,
                               size_t      texture_index)
{
    fig.set_texture_index(texture_index);
    figures.push_back(&fig);
};
