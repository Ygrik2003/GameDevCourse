#include "game.h"
#include "objects/model.h"

game_tetris::game_tetris()
{
    state.is_started = 1;
    state.is_quit    = 0;
    state.is_rotated = 0;
    state.is_moving  = 0;

    figure_board = model("./99-game/textures/board.obj").get_figure();
    figure_cube  = model("./99-game/textures/cube.obj").get_figure();

    buffer_z.resize(cells_max * cells_max);

    columns.resize(cells_max * cells_max * cells_max_z);
    for (column& col : columns)
    {
        col = column{ 0 };
    }
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
    if (!my_engine->initialize(this->cfg))
        return -1;

    shader_scene = new shader_opengl(cfg.shader_vertex, cfg.shader_fragment);
    my_engine->set_shader(shader_scene);

    texture_block = my_engine->load_texture(1, cfg.texture_block);
    texture_board = my_engine->load_texture(2, cfg.texture_board);

    add_figure(figure_board, texture_board);
    add_primitive();
    my_engine->play_sound("./99-game/res/road.wav");
    // my_engine->play_sound("./99-game/res/8-bit_detective.wav");

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

        // Only game buttons
        if (state.is_started)
        {
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
                camera_angle += e.motion.x * M_PI / 300;
            }
            if (e.motion.y && state.is_rotated)
            {
                view_height += e.motion.y / 50;
                if (view_height < 1.)
                    view_height = 1.;
                if (view_height > 1.6)
                    view_height = 1.6;
            }
            if (e.keyboard.w_clicked)
            {
            }
            if (e.keyboard.s_clicked)
            {
            }
            if (e.keyboard.a_clicked)
            {
            }
            if (e.keyboard.d_clicked)
            {
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
    static std::chrono::steady_clock timer;
    static auto                      last_time_update = timer.now();

    cam->set_rotate(
        0, M_PI / 2 + camera_angle, M_PI / 2 - atan(1. / sqrt(view_height)));
    cam->set_translate(sqrt(view_height) * std::cos(camera_angle),
                       -view_height,
                       -sqrt(view_height) * std::sin(camera_angle));

    if ((timer.now() - last_time_update).count() < delay * 1e9)
        return;
    last_time_update = timer.now();

    for (int x = 0; x < cells_max; x++)
    {
        for (int y = 0; y < cells_max; y++)
        {
            if (get_cell_color(x, y, get_column_z(x, y)) != 0)
            {
                update_buffer_z();
                add_primitive();
                // my_engine->play_sound("./99-game/res/metal_pipe.wav");
            }
            get_column(x, y).move_down_over(get_column_z(x, y));
        }
    }
}

void game_tetris::render()
{
    ImGui::NewFrame();
    if (!state.is_started)
    {
        draw_menu();
    }
    else
    {
        shader_scene->use();
        render_scene();
        draw_ui();
    }
    ImGui::Render();
    my_engine->swap_buffers();
};
void game_tetris::add_figure(figure* fig, texture* tex)
{
    fig->set_texture(tex);
    figures.push_back(fig);
}
void game_tetris::draw_menu()
{
    static const int window_width  = 0.2 * cfg.width;
    static const int window_height = 0.2 * cfg.height;
    static const int window_x      = (cfg.width - window_width) / 2;
    static const int window_y      = (cfg.height - window_height) / 2;

    ImGui::SetNextWindowSize(ImVec2(window_width, window_height));
    ImGui::SetNextWindowPos(ImVec2(window_x, window_y));

    ImGui::Begin("Menu",
                 0,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);

    if (ImGui::Button("Start", ImVec2(0.1 * cfg.width, 0.05 * cfg.height)))
    {
        start_game();
    }
    if (ImGui::Button("Settings", ImVec2(0.15 * cfg.width, 0.05 * cfg.height)))
    {
    }
    if (ImGui::Button("Quit", ImVec2(0.09 * cfg.width, 0.05 * cfg.height)))
    {
    }

    ImGui::End();
}
void game_tetris::draw_ui()
{
    static const int window_score_width  = 0.1 * cfg.width;
    static const int window_score_height = 0.03 * cfg.height;
    static const int window_score_x      = 10;
    static const int window_score_y      = 10;

    static const int window_control_width  = 0.2 * cfg.width;
    static const int window_control_height = 0.95 * cfg.height;
    static const int window_control_x      = 0.8 * cfg.width - 10;
    static const int window_control_y      = 10;

    ImGui::SetNextWindowSize(ImVec2(window_score_width, window_score_height));
    ImGui::SetNextWindowPos(ImVec2(window_score_x, window_score_y));

    ImGui::Begin("State",
                 0,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);

    ImGui::Text("Score: %d", score);

    ImGui::End();

    ImGui::SetNextWindowSize(
        ImVec2(window_control_width, window_control_height));
    ImGui::SetNextWindowPos(ImVec2(window_control_x, window_control_y));

    ImGui::Begin("Controller",
                 0,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);

    if (ImGui::Button("Forvard", ImVec2(100, 100)))
    {
    }
    ImGui::SameLine();
    // ImGui::SetCursorPos(ImVec2(100, 100));
    if (ImGui::Button("Backward", ImVec2(100, 100)))
    {
    }
    if (ImGui::Button("Left", ImVec2(100, 100)))
    {
    }
    ImGui::SameLine();
    if (ImGui::Button("Right", ImVec2(100, 100)))
    {
    }

    if (ImGui::Button("Rotate X axis", ImVec2(window_control_width - 20, 100)))
    {
    }
    if (ImGui::Button("Rotate Y axis", ImVec2(window_control_width - 20, 100)))
    {
    }
    if (ImGui::Button("Rotate Z axis", ImVec2(window_control_width - 20, 100)))
    {
    }

    ImGui::End();
}
void game_tetris::render_scene()
{

    for (figure* fig : figures)
    {
        fig->uniform_link(uniforms);

        vertex_buffer<vertex3d_textured>* vertex_buff = new vertex_buffer(
            fig->get_vertexes().data(), fig->get_vertexes().size());
        index_buffer* index_buff = new index_buffer(fig->get_indexes().data(),
                                                    fig->get_indexes().size());
        my_engine->reload_uniform();
        my_engine->render_triangles(
            vertex_buff, index_buff, fig->get_texture(), 0, index_buff->size());

        delete vertex_buff;
        delete index_buff;
    }
    vertex_buffer<vertex3d_textured>* vertex_buff = new vertex_buffer(
        figure_cube->get_vertexes().data(), figure_cube->get_vertexes().size());
    index_buffer* index_buff = new index_buffer(
        figure_cube->get_indexes().data(), figure_cube->get_indexes().size());

    for (int x = 0; x < cells_max; x++)
        for (int y = 0; y < cells_max; y++)
        {
            for (uint16_t z = 0; z < column_count_cells; z++)
            {
                if (!get_column(x, y).get_z(z))
                    continue;
                figure_cube->set_translate(
                    vector3d(-1. / 2. + (x + 0.5) / cells_max,
                             (z + 0.5) / cells_max,
                             -1. / 2. + (y + 0.5) / cells_max));
                figure_cube->set_texture(texture_block);
                figure_cube->uniform_link(uniforms);

                my_engine->reload_uniform();
                my_engine->render_triangles(vertex_buff,
                                            index_buff,
                                            figure_cube->get_texture(),
                                            0,
                                            index_buff->size());
            }
        }

    delete vertex_buff;
    delete index_buff;
}
void game_tetris::start_game()
{
    state.is_started = ~state.is_started;
}

void game_tetris::add_primitive()
{
    static float x = 0;
    static float y = 0;

    set_cell_color(x, y, cells_max_z, 1);
    x++;
    if (x == cells_max)
    {
        x = 0;
        y += 1;
        if (y == cells_max)
            y = 0;
    }
}

void game_tetris::set_cell_color(uint8_t x, uint8_t y, uint8_t z, uint8_t clr)
{
    columns[y * cells_max + x] |= static_cast<uint32_t>(clr)
                                  << (column_bit_for_color * z);
}

uint8_t game_tetris::get_cell_color(uint8_t x, uint8_t y, uint8_t z)
{
    return get_column(x, y).get_z(z);
}

column& game_tetris::get_column(uint8_t x, uint8_t y)
{
    return columns[y * cells_max + x];
}

uint8_t game_tetris::get_column_z(uint8_t x, uint8_t y)
{
    return buffer_z[y * cells_max + x];
}

void game_tetris::set_column_z(uint8_t x, uint8_t y, uint8_t byte)
{
    buffer_z[y * cells_max + x] = byte;
}

void game_tetris::update_buffer_z()
{
    for (int x = 0; x < cells_max; x++)
    {
        for (int y = 0; y < cells_max; y++)
        {
            for (int z = get_column_z(x, y); z < cells_max_z; z++)
            {
                if (get_cell_color(x, y, z))
                    set_column_z(x, y, z + 1);
            }
        }
    }
}
