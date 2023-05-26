#pragma once
#include "core/config.h"
#include "core/event.h"
#include "objects/camera.h"
#include "objects/figure.h"

#include <iostream>
#include <vector>

#include <KHR/khrplatform.h>
#include <SDL3/SDL.h>
#include <glad/glad.h>

class engine
{
public:
    virtual ~engine() = default;

    virtual int  initialize(config _config) = 0;
    virtual void uninitialize()             = 0;

    virtual bool event_keyboard(event&) = 0;

    virtual void render_triangle(const triangle<vertex>& tr)          = 0;
    virtual void render_triangle(const triangle<vertex_colored>& tr)  = 0;
    virtual void render_triangle(const triangle<vertex_textured>& tr) = 0;
    virtual void render_triangle(
        const triangle<vertex_colored_textured>& tr) = 0;
    virtual void swap_buffers()                      = 0;

    virtual void load_shader(const char* path, int type)     = 0;
    virtual void reload_shader(const char* path_to_vertex,
                               const char* path_to_fragment) = 0;

    virtual void create_shadow_map() = 0;

    virtual void load_texture(size_t index, const char* path) = 0;
    virtual void set_texture(size_t index)                    = 0;

    virtual figure load_object(const char* path) = 0;

protected:
    config _config;
};

class engine_tetris : engine
{
public:
    int  initialize(config _config) override;
    void uninitialize() override;

    bool event_keyboard(event&) override;

    void render_triangle(const triangle<vertex>& tr) override;
    void render_triangle(const triangle<vertex_colored>& tr) override;
    void render_triangle(const triangle<vertex_textured>& tr) override;
    void render_triangle(const triangle<vertex_colored_textured>& tr) override;
    void swap_buffers() override;

    void reload_shader(const char* path_to_vertex,
                       const char* path_to_fragment) override;

    void load_texture(size_t index, const char* path) override;
    void set_texture(size_t index) override;

    figure load_object(const char* path) override;

    void create_shadow_map() override;

    void set_uniform(uniform& uni);
    void reload_uniform();

private:
    void load_shader(const char* path, int type) override;

    SDL_Window*   window;
    SDL_GLContext gl_context;

    GLuint shader_program = 0;

    GLuint obj_depth_map     = 0;
    GLuint texture_depth_map = 0;

    GLuint uniforms        = 0;
    GLuint uniform_texture = 0;

    uniform* uniforms_world;
};