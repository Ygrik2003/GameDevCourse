#include "engine.h"

bool ImGui_ImplSdlGL3_Init(SDL_Window* window);
void ImGui_ImplSdlGL3_Shutdown();
void ImGui_ImplSdlGL3_InvalidateDeviceObjects();
bool ImGui_ImplSdlGL3_CreateDeviceObjects();
void ImGui_ImplSdlGL3_NewFrame(SDL_Window* window);
bool ImGui_ImplSdlGL3_ProcessEvent(const SDL_Event* event);
void ImGui_ImplSdlGL3_RenderDrawLists(engine* eng, ImDrawData* draw_data);

class engine_opengl : engine
{
public:
    int  initialize(config& _config) override;
    void uninitialize() override;

    bool event_keyboard(event&) override;

    void render_triangle(const triangle<vertex3d>& tr) override;
    void render_triangle(const triangle<vertex3d_colored>& tr) override;
    void render_triangle(const triangle<vertex3d_textured>& tr) override;
    void render_triangle(
        const triangle<vertex3d_colored_textured>& tr) override;

    void render_triangles(vertex_buffer<vertex3d>* vertexes,
                          index_buffer*            indexes,
                          const std::uint16_t*     start_vertex_index,
                          size_t                   num_vertexes) override;
    void render_triangles(vertex_buffer<vertex3d_colored>* vertexes,
                          index_buffer*                    indexes,
                          const std::uint16_t*             start_vertex_index,
                          size_t num_vertexes) override;
    void render_triangles(vertex_buffer<vertex3d_textured>* vertexes,
                          index_buffer*                     indexes,
                          const texture_opengl*             tex,
                          const std::uint16_t*              start_vertex_index,
                          size_t num_vertexes) override;
    void render_triangles(vertex_buffer<vertex3d_colored_textured>* vertexes,
                          index_buffer*                             indexes,
                          const texture_opengl*                     tex,
                          const std::uint16_t* start_vertex_index,
                          size_t               num_vertexes) override;
    void render_triangles(vertex_buffer<vertex2d_colored_textured>* vertexes,
                          index_buffer*                             indexes,
                          const texture_opengl*                     tex,
                          const std::uint16_t* start_vertex_index,
                          size_t               num_vertexes);

    void swap_buffers() override;

    texture* load_texture(uint32_t index, const char* path) override;
    void     set_texture(uint32_t index) override;

    void set_uniform(uniform& uni);
    void reload_uniform();

    void set_shader(shader* shader);
    void set_relative_mouse_mode(bool state);

private:
    SDL_Window* window = nullptr;

    shader* active_shader = nullptr;

    GLuint obj_depth_map     = 0;
    GLuint texture_depth_map = 0;

    uniform* uniforms_world = nullptr;
};
