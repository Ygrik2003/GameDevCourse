#pragma once
#include "core/config.h"
#include "core/event.h"
#include "objects/camera.h"

#include <vector>

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#include <glm/glm.hpp>

class engine
{
public:
    virtual ~engine() = default;

    virtual int  initialize(config _config) = 0;
    virtual void uninitialize()             = 0;

    virtual bool event_keyboard(event&) = 0;

    virtual void render_triangle(const triangle<vertex_textured>& tr) = 0;
    virtual void swap_buffers()                                       = 0;

    virtual void load_shader(const char* path, int type)   = 0;
    virtual void reload_shader(const char* path, int type) = 0;

    virtual void load_texture(const char* path) = 0;

protected:
    config _config;
};

class engine_checkers : engine
{
public:
    int  initialize(config _config) override;
    void uninitialize() override;

    bool event_keyboard(event&) override;

    void render_triangle(const triangle<vertex_textured>& tr) override;
    void swap_buffers() override;

    void reload_shader(const char* path, int type) override;

    void load_texture(const char* path) override;

    void set_uniform(uniform& uni);

private:
    void load_shader(const char* path, int type) override;

    SDL_Window*   window;
    SDL_GLContext gl_context;

    GLuint program = 0;

    GLuint shader_vertex                    = 0;
    GLuint shader_fragment_chessboard_cells = 0;
    GLuint shader_fragment_chessboard       = 0;
    GLuint shader_fragment_checker          = 0;

    GLuint uniforms        = 0;
    GLuint uniform_texture = 0;

    uniform* uniforms_world;
};