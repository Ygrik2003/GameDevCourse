#include "game.h"

int game_tetris::initialize(config cfg)
{
    this->cfg = cfg;

    uniforms.width  = cfg.width;
    uniforms.height = cfg.height;

    cam = new camera(cfg.camera_speed);
    cam->uniform_link(uniforms);
    // cam->move(0.6, -0.1, 0.6);
    cam->set_rotate(0, 0, 0);

    my_engine = new engine_tetris();
    my_engine->set_uniform(uniforms);
    my_engine->initialize(cfg);

    my_engine->load_texture(0, cfg.texture_cells);
    my_engine->load_texture(1, cfg.texture_board);
    my_engine->load_texture(2, cfg.texture_checker_white);
    my_engine->load_texture(3, cfg.texture_checker_black);

    return 1;
};

bool game_tetris::event_listener(event& e)
{
    while (my_engine->event_keyboard(e))
    {
        if (e.action.quit)
        {
            return false;
        }
        else if (e.motion.x || e.motion.y)
        {
            cam->add_rotate(0,
                            cfg.camera_speed_rotate * e.motion.x,
                            cfg.camera_speed_rotate * e.motion.y);
        }
        else if (e.keyboard.up_clicked)
            cam->set_move_forward(true);
        else if (e.keyboard.up_released)
            cam->set_move_forward(false);
        if (e.keyboard.down_clicked)
            cam->set_move_backward(true);
        else if (e.keyboard.down_released)
            cam->set_move_backward(false);
        if (e.keyboard.left_clicked)
            cam->set_move_left(true);
        else if (e.keyboard.left_released)
            cam->set_move_left(false);
        if (e.keyboard.right_clicked)
            cam->set_move_right(true);
        else if (e.keyboard.right_released)
            cam->set_move_right(false);
        else if (e.keyboard.space_clicked)
        {
            my_engine->reload_shader(cfg.shader_vertex, cfg.shader_fragment);
        }
    }
    return true;
};

void game_tetris::update()
{
    cam->update();

    phi += M_PI / 300;
    theta += M_PI / 300;

    cam->add_rotate(0, -M_PI / 300, 0);
    cam->set_translate(1 * std::cos(M_PI + phi), -.5, 1 * std::sin(M_PI + phi));
};

void game_tetris::render()
{

    for (auto fig : figures)
    {
        fig.uniform_link(uniforms);
        // fig.set_rotate(0, 0, theta);
        my_engine->set_texture(fig.get_texture_index());
        for (size_t i = 0; i < fig.get_count(); i++)
        {
            triangle tr = fig.get_triangle(i);
            my_engine->render_triangle(tr);
        }
    }
    my_engine->swap_buffers();
};

void game_tetris::add_figure(figure      fig,
                             const char* texture,
                             size_t      texture_index)
{

    fig.set_texture_index(texture_index);
    figures.push_back(fig);
};
