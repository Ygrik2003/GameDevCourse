#pragma once
#include "core/config.h"
#include "core/event.h"
#include "index_buffer.h"
#include "objects/camera.h"
#include "objects/figure.h"
#include "shader_opengl.h"
#include "texture_opengl.h"
#include "vertex_buffer.h"

#include <iostream>
#include <vector>

#ifndef _WIN32
#include <KHR/khrplatform.h>
#endif
#include <SDL3/SDL.h>
#include <glad/glad.h>

#include "imgui/imgui.h"

// Use if you want to reset your rendering device without losing ImGui state.
void ImGui_ImplSdlGL3_InvalidateDeviceObjects();
bool ImGui_ImplSdlGL3_CreateDeviceObjects();

class engine
{
public:
    virtual ~engine() = default;

    virtual int  initialize(config& _config) = 0;
    virtual void uninitialize()              = 0;

    virtual bool event_keyboard(event&) = 0;

    virtual void render_triangle(const triangle<vertex3d>& tr)          = 0;
    virtual void render_triangle(const triangle<vertex3d_colored>& tr)  = 0;
    virtual void render_triangle(const triangle<vertex3d_textured>& tr) = 0;
    virtual void render_triangle(
        const triangle<vertex3d_colored_textured>& tr) = 0;

    virtual void render_triangles(vertex_buffer<vertex3d>*,
                                  index_buffer*,
                                  const std::uint16_t*,
                                  size_t) = 0;
    virtual void render_triangles(vertex_buffer<vertex3d_colored>*,
                                  index_buffer*,
                                  const std::uint16_t*,
                                  size_t) = 0;
    virtual void render_triangles(vertex_buffer<vertex3d_textured>*,
                                  index_buffer*,
                                  const texture_opengl*,
                                  const std::uint16_t*,
                                  size_t) = 0;
    virtual void render_triangles(vertex_buffer<vertex3d_colored_textured>*,
                                  index_buffer*,
                                  const texture_opengl*,
                                  const std::uint16_t*,
                                  size_t) = 0;
    virtual void render_triangles(
        vertex_buffer<vertex2d_colored_textured>* vertexes,
        index_buffer*                             indexes,
        const texture_opengl*                     tex,
        const std::uint16_t*                      start_vertex_index,
        size_t                                    num_vertexes) = 0;

    virtual void swap_buffers() = 0;

    virtual void create_shadow_map() = 0;

    virtual texture_opengl* load_texture(uint32_t index, const char* path) = 0;
    virtual void            set_texture(uint32_t index)                    = 0;

protected:
    config _config;
};

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

    texture_opengl* load_texture(uint32_t index, const char* path) override;
    void            set_texture(uint32_t index) override;

    void create_shadow_map() override;

    void set_uniform(uniform& uni);
    void reload_uniform();

    void set_shader(shader_opengl* shader);

private:
    SDL_Window*   window = nullptr;
    SDL_GLContext gl_context;

    shader_opengl* active_shader = nullptr;

    GLuint obj_depth_map     = 0;
    GLuint texture_depth_map = 0;

    uniform* uniforms_world = nullptr;
};

bool ImGui_ImplSdlGL3_Init(SDL_Window* window);
void ImGui_ImplSdlGL3_Shutdown();
void ImGui_ImplSdlGL3_NewFrame(SDL_Window* window);
bool ImGui_ImplSdlGL3_ProcessEvent(const SDL_Event* event);
void ImGui_ImplSdlGL3_RenderDrawLists(engine* eng, ImDrawData* draw_data);