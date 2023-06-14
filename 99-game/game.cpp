#include "game.h"

game_tetris::game_tetris()
{
    state.is_started = 1;
    state.is_quit    = 0;
    state.is_rotated = 0;
    state.is_moving  = 0;

    last_time_update = timer.now();

    figure_board = model("./99-game/textures/board.obj").get_figure();
    figure_cube  = model("./99-game/textures/cube.obj").get_figure();

    buffer_z.resize(cells_max * cells_max);

    cells.resize(cells_max * cells_max * cells_max_z);
    uint8_t x = 0, y = 0, z = 0;
    for (cell& c : cells)
    {
        c.is_free = true;
        c.x       = x;
        c.y       = y;
        c.z       = z;
        x++;
        if (x == cells_max)
        {
            x = 0;
            y++;
            if (y == cells_max)
            {
                y = 0;
                z++;
            }
        }
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
    new_primitive();

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
                phi += e.motion.x * M_PI / 300;
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
                move_cell(controlled_cell, direction::forward);
            }
            if (e.keyboard.s_clicked)
            {
                move_cell(controlled_cell, direction::backward);
            }
            if (e.keyboard.a_clicked)
            {
                move_cell(controlled_cell, direction::left);
            }
            if (e.keyboard.d_clicked)
            {
                move_cell(controlled_cell, direction::right);
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
    // cam->update();
    cam->set_rotate(0, M_PI / 2 + phi, M_PI / 2 - atan(1. / sqrt(view_height)));
    cam->set_translate(sqrt(view_height) * std::cos(phi),
                       -view_height,
                       -sqrt(view_height) * std::sin(phi));

    if ((timer.now() - last_time_update).count() < 1.5e7)
        return;
    last_time_update = timer.now();
    for (cell& c : cells)
    {
        if (c.is_free || !c.is_moving)
            continue;

        if (c.z == buffer_z[c.y * cells_max + c.x])
        {
            buffer_z[c.y * cells_max + c.x]++;
            c.set_moving(false);
            check_layers(c);
            if (c.is_controlled)
            {
                c.set_controlling(false);
                new_primitive();
            }
        }
        else
        {
            move_cell(&c, direction::down);
        }
    }
};

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

    static const int window_width  = 0.1 * cfg.width;
    static const int window_height = 0.05 * cfg.height;
    static const int window_x      = 10;
    static const int window_y      = 10;

    ImGui::SetNextWindowSize(ImVec2(window_width, window_height));
    ImGui::SetNextWindowPos(ImVec2(window_x, window_y));

    ImGui::Begin("State",
                 0,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);

    ImGui::Text("Score: %d", score);

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

    for (cell& c : cells)
    {
        if (c.is_free)
            continue;
        figure_cube->set_translate(
            vector3d(-1. / 2. + (static_cast<float>(c.x) + 0.5) / cells_max,
                     (static_cast<float>(c.z) + 0.5) / cells_max,
                     -1. / 2. + (static_cast<float>(c.y) + 0.5) / cells_max));
        figure_cube->set_texture(texture_block);
        figure_cube->uniform_link(uniforms);

        my_engine->reload_uniform();
        my_engine->render_triangles(vertex_buff,
                                    index_buff,
                                    figure_cube->get_texture(),
                                    0,
                                    index_buff->size());
    }

    delete vertex_buff;
    delete index_buff;
}
void game_tetris::start_game()
{
    state.is_started = ~state.is_started;
}
cell& game_tetris::get_cell(size_t x, size_t y, size_t z)
{
    return *std::find_if(cells.begin(),
                         cells.end(),
                         [&](const cell& c)
                         { return (c.x == x) && (c.y == y) && (c.z == z); });
}
bool game_tetris::move_cell(cell* c, direction dir)
{

    switch (dir)
    {
        case direction::down:
            if (c->z == 0)
                return false;
            break;
        case direction::up:
            if (c->z == cells_max_z - 1)
                return false;
            break;
        case direction::left:
            if (c->x == 0 || !get_cell(c->x - 1, c->y, c->z).is_free)
                return false;
            break;
        case direction::right:
            if (c->x == cells_max - 1 ||
                !get_cell(c->x + 1, c->y, c->z).is_free)
                return false;
            break;
        case direction::forward:
            if (c->y == cells_max - 1 ||
                !get_cell(c->x, c->y + 1, c->z).is_free)
                return false;
            break;
        case direction::backward:
            if (c->y == 0 || !get_cell(c->x, c->y - 1, c->z).is_free)
                return false;
            break;
    }

    if (c->next)
        if (!move_cell(c->next, dir))
            return false;

    if (c->prev)
        if (!move_cell(c->prev, dir))
            return false;

    cell* cell_to_swap;
    switch (dir)
    {
        case direction::down:
            cell_to_swap = &get_cell(c->x, c->y, c->z - 1);
            c->z--;
            cell_to_swap->z++;
            std::swap(c, cell_to_swap);
            break;
        case direction::up:
            cell_to_swap = &get_cell(c->x, c->y, c->z + 1);
            c->z++;
            cell_to_swap->z--;
            std::swap(c, cell_to_swap);
            break;
        case direction::left:
            cell_to_swap = &get_cell(c->x - 1, c->y, c->z);
            c->x--;
            cell_to_swap->x++;
            std::swap(c, cell_to_swap);
            break;
        case direction::right:
            cell_to_swap = &get_cell(c->x + 1, c->y, c->z);
            c->x++;
            cell_to_swap->x--;
            std::swap(c, cell_to_swap);
            break;
        case direction::forward:
            cell_to_swap = &get_cell(c->x, c->y + 1, c->z);
            c->y++;
            cell_to_swap->y--;
            std::swap(c, cell_to_swap);
            break;
        case direction::backward:
            cell_to_swap = &get_cell(c->x, c->y - 1, c->z);
            c->y--;
            cell_to_swap->y++;
            std::swap(c, cell_to_swap);
            break;
    }
    return true;
}
void game_tetris::new_primitive()
{
    // controlled_cell = &get_cell(
    //     std::rand() % cells_max, std::rand() % cells_max, cells_max_z - 1);
    static float x  = 0;
    static float y  = 0;
    controlled_cell = &get_cell((int)x, (int)y, cells_max_z - 1);
    x++;
    if (x == cells_max)
    {
        x = 0;
        y += 1;
        if (y == cells_max)
            y = 0;
    }
    controlled_cell->is_free = false;
    controlled_cell->set_controlling(true);
    controlled_cell->set_moving(true);
}

void game_tetris::check_layers(cell& last_cell)
{
    for (size_t x = 0; x < cells_max; x++)
        for (size_t y = 0; y < cells_max; y++)
        {
            if (get_cell(x, y, last_cell.z).is_free)
                return;
        }
    score++;
    for (cell& c : cells)
    {
        if (c.z == last_cell.z)
        {
            buffer_z[c.y * cells_max + c.x] = c.z;
            c.is_free                       = true;
        }
        c.set_moving(true);
    }
    if (last_cell.next)
        check_layers(*last_cell.next);
    if (last_cell.prev)
        check_layers(*last_cell.prev);
}
