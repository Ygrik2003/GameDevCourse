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

    my_engine = new engine_opengl();

    my_engine->set_uniform(uniforms);
    my_engine->initialize(this->cfg);

    shader_scene = new shader_opengl(cfg.shader_vertex, cfg.shader_fragment);
    // shader_temp = new shader_opengl(cfg.shader_vertex,
    //                                 "./99-game/shaders/shader_test.frag");
    my_engine->set_shader(shader_scene);

    textures.push_back(my_engine->load_texture(0, cfg.texture_cells));
    textures.push_back(my_engine->load_texture(1, cfg.texture_board));
    textures.push_back(my_engine->load_texture(2, cfg.texture_checker_white));
    textures.push_back(my_engine->load_texture(3, cfg.texture_checker_black));

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
            shader_scene->reload();
            isStarted = !isStarted;
        }
    }
    return true;
};

void game_tetris::update()
{
    cam->update();

    phi += M_PI / 300;
    theta += M_PI / 300;

    // cam->add_rotate(0, -M_PI / 300, 0);
    // cam->set_translate(1 * std::cos(M_PI + phi), -.5, 1 * std::sin(M_PI +
    // phi));
};

void game_tetris::render()
{
    ImGui::NewFrame();
    if (!isStarted)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,
                            ImVec2(cfg.width - 20, cfg.height - 20));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
        ImGui::Begin("Menu");
        render_menu();
        ImGui::End();
        ImGui::PopStyleVar(2);
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

    ImGui::Text("This is some useful text."); // Display some text (you can use
                                              // a format strings too)
    ImGui::Checkbox(
        "Demo Window",
        &isStarted); // Edit bools storing our window open/close state

    // ImGui::ColorEdit3(
    //     "clear color",
    //     (float*)&clear_color); // Edit 3 floats representing a color

    if (ImGui::Button("Button")) // Buttons return true when clicked (most
                                 // widgets return true when edited/activated)
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / io.Framerate,
                io.Framerate);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / io.Framerate,
                io.Framerate);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / io.Framerate,
                io.Framerate);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / io.Framerate,
                io.Framerate);
}
void game_tetris::render_scene()
{

    for (auto fig : figures)
    {
        fig.uniform_link(uniforms);
        // fig.set_rotate(0, 0, theta);

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

        // my_engine->set_texture(fig.get_texture_index());
        // for (size_t i = 0; i < fig.get_count(); i++)
        // {
        //     triangle tr = fig.get_triangle(i);
        //     my_engine->render_triangle(tr);
        // }
    }
}
