#include "game.h"

int game_checkers::initialize(config cfg)
{
    this->cfg = cfg;

    uniforms;
    uniforms.width  = cfg.width;
    uniforms.height = cfg.height;

    cam = camera();
    cam.uniform_link(uniforms);
    cam.move(0, 0, -5);

    my_engine = new engine_checkers();
    my_engine->set_uniform(uniforms);
    my_engine->initialize(cfg);

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
        else if (e.keyboard.up_clicked)
        {
            cam.move_forward(cfg.camera_speed);
        }
        else if (e.keyboard.down_clicked)
        {
            cam.move_backward(cfg.camera_speed);
        }
        else if (e.keyboard.left_clicked)
        {
            cam.move_left(cfg.camera_speed);
        }
        else if (e.keyboard.right_clicked)
        {

            cam.move_right(cfg.camera_speed);
        }
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
            my_engine->reload_shader(cfg.shader_fragment_chessboard_cells,
                                     GL_FRAGMENT_SHADER);
        }
    }
    return true;
};

void game_checkers::update(){

};

void game_checkers::render()
{

    for (auto fig : figures)
    {
        fig->uniform_link(uniforms);
        for (size_t i = 0; i < fig->get_count(); i++)
        {
            triangle tr = fig->get_triangle(i);
            my_engine->render_triangle(tr);
        }
    }
    my_engine->swap_buffers();
};

void game_checkers::add_figure(figure& fig)
{
    figures.push_back(&fig);
};
