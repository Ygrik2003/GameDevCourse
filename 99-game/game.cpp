#include "game.h"

game_tetris::game_tetris()
{
    state.is_started = true;
    state.is_quit    = false;
    state.is_rotated = false;
    state.is_moving  = false;
}

int game_tetris::initialize(config cfg)
{

    this->cfg = cfg;

    uniforms.width  = cfg.width;
    uniforms.height = cfg.height;

    cam = new camera(cfg.camera_speed);
    cam->uniform_link(uniforms);

    my_engine = new engine_opengl();

    my_engine->set_uniform(uniforms);
    my_engine->initialize(this->cfg);

    shader_scene = new shader_opengl(cfg.shader_vertex, cfg.shader_fragment);
    // shader_temp = new shader_opengl(cfg.shader_vertex,
    //                                 "./99-game/shaders/shader_test.frag");
    my_engine->set_shader(shader_scene);

    textures.push_back(my_engine->load_texture(0, cfg.texture_block));
    textures.push_back(my_engine->load_texture(1, cfg.texture_board));

    return 1;
};

bool game_tetris::event_listener(event& e)
{
    e.clear();
    if (my_engine->event_keyboard(e))
    {
        // Always listen buttons
        if (e.action.quit)
        {
            return false;
        }
        // else if (e.keyboard.space_clicked)
        // {
        //     shader_scene->reload();
        //     state.is_started = !state.is_started;
        // }

        // Only game buttons
        if (state.is_started)
        {
            // std::cout << static_cast<int>(state.is_rotated) << std::endl;
            if (e.mouse.left_cliked)
            {
                state.is_rotated = true;
                state.is_moving  = true;
            }
            if (e.mouse.left_released)
            {
                state.is_rotated = false;
                state.is_moving  = false;
            }
            if (e.motion.x && state.is_rotated)
            {
                phi += e.motion.x * M_PI / 300;
            }
            if (e.motion.y && state.is_rotated)
            {
                view_height += e.motion.y / 50;
                if (view_height < 1.5)
                    view_height = 1.5;
            }
            // Free Camera
            // if (e.motion.x || e.motion.y)
            // {
            //     cam->add_rotate(0,
            //                     cfg.camera_speed_rotate * e.motion.x,
            //                     cfg.camera_speed_rotate * e.motion.y);
            // }
            // if (e.keyboard.w_clicked)
            //     cam->set_move_forward(true);
            // if (e.keyboard.w_released)
            //     cam->set_move_forward(false);
            // if (e.keyboard.s_clicked)
            //     cam->set_move_backward(true);
            // if (e.keyboard.s_released)
            //     cam->set_move_backward(false);
            // if (e.keyboard.a_clicked)
            //     cam->set_move_left(true);
            // if (e.keyboard.a_released)
            //     cam->set_move_left(false);
            // if (e.keyboard.d_clicked)
            //     cam->set_move_right(true);
            // if (e.keyboard.d_released)
            //     cam->set_move_right(false);
        }
    }
    return true;
};

void game_tetris::update()
{
    cam->update();

    cam->set_rotate(0, M_PI / 2 + phi, M_PI / 4);
    cam->set_translate(1.5 * std::cos(phi), -view_height, -1.5 * std::sin(phi));
    // theta += M_PI / 300;
};

void game_tetris::render()
{
    ImGui::NewFrame();
    if (!state.is_started)
    {
        ImGui::Begin("Menu",
                     0,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoMove |
                         ImGuiWindowFlags_NoScrollbar);
        // ImGui::SetWindowPos(ImVec2(cfg.width / 2 - 200, cfg.height / 2 -
        // 200));
        ImGui::SetWindowSize(ImVec2(400, 400));

        render_menu();

        ImGui::End();
    }
    else
    {
        shader_scene->use();
        render_scene();
    }
    ImGui::Render();
    my_engine->swap_buffers();
};

void game_tetris::add_figure(figure      fig,
                             const char* texture,
                             size_t      texture_index)
{

    fig.set_texture_index(texture_index);
    figures.push_back(fig);
}
void game_tetris::render_menu()
{
    static float f       = 0.0f;
    static int   counter = 0;

    ImGuiIO& io = ImGui::GetIO();

    if (ImGui::Button("Start", ImVec2(100, 30)))
    {
        start_game();
    }
    if (ImGui::Button("Settings"))
    {
    }
    // ImGui::SameLine();
    if (ImGui::Button("Quit"))
    {
    }
}
void game_tetris::render_scene()
{

    for (auto fig : figures)
    {
        fig.uniform_link(uniforms);

        vertex_buffer<vertex3d_textured>* vertex_buff = new vertex_buffer(
            fig.get_vertexes().data(), fig.get_vertexes().size());
        index_buffer* index_buff = new index_buffer(fig.get_indexes().data(),
                                                    fig.get_indexes().size());
        my_engine->reload_uniform();
        my_engine->render_triangles(vertex_buff,
                                    index_buff,
                                    textures[fig.get_texture_index()],
                                    0,
                                    index_buff->size());

        delete vertex_buff;
        delete index_buff;

        // TODO test speed

        // my_engine->set_texture(fig.get_texture_index());
        // for (size_t i = 0; i < fig.get_count(); i++)
        // {
        //     triangle tr = fig.get_triangle(i);
        //     my_engine->render_triangle(tr);
        // }
    }
}

void game_tetris::start_game()
{
    state.is_started = !state.is_started;
}
