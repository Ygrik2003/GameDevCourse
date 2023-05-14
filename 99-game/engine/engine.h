#pragma once
#include "core/config.h"
#include "core/event.h"
#include "core/types.h"

#include <vector>

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_opengl_glext.h>
#include <glm/glm.hpp>

PFNGLCREATESHADERPROC            glCreateShader            = nullptr;
PFNGLSHADERSOURCEPROC            glShaderSource            = nullptr;
PFNGLCOMPILESHADERPROC           glCompileShader           = nullptr;
PFNGLGETSHADERIVPROC             glGetShaderiv             = nullptr;
PFNGLGETSHADERINFOLOGPROC        glGetShaderInfoLog        = nullptr;
PFNGLDELETESHADERPROC            glDeleteShader            = nullptr;
PFNGLCREATEPROGRAMPROC           glCreateProgram           = nullptr;
PFNGLATTACHSHADERPROC            glAttachShader            = nullptr;
PFNGLBINDATTRIBLOCATIONPROC      glBindAttribLocation      = nullptr;
PFNGLLINKPROGRAMPROC             glLinkProgram             = nullptr;
PFNGLGETPROGRAMIVPROC            glGetProgramiv            = nullptr;
PFNGLGETPROGRAMINFOLOGPROC       glGetProgramInfoLog       = nullptr;
PFNGLDELETEPROGRAMPROC           glDeleteProgram           = nullptr;
PFNGLUSEPROGRAMPROC              glUseProgram              = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC     glVertexAttribPointer     = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = nullptr;
PFNGLVALIDATEPROGRAMPROC         glValidateProgram         = nullptr;
PFNGLDEBUGMESSAGECONTROLPROC     glDebugMessageControl     = nullptr;
PFNGLDEBUGMESSAGECALLBACKPROC    glDebugMessageCallback    = nullptr;
PFNGLGENBUFFERSPROC              glGenBuffers              = nullptr;
PFNGLBINDBUFFERPROC              glBindBuffer              = nullptr;
PFNGLGENVERTEXARRAYSPROC         glGenVertexArrays         = nullptr;
PFNGLBINDVERTEXARRAYPROC         glBindVertexArray         = nullptr;
PFNGLGETUNIFORMLOCATIONPROC      glGetUniformLocation      = nullptr;
PFNGLUNIFORM1FPROC               glUniform1f               = nullptr;
PFNGLUNIFORM1IPROC               glUniform1i               = nullptr;
PFNGLUNIFORM1FVPROC              glUniform1fv              = nullptr;
PFNGLBUFFERDATAPROC              glBufferData              = nullptr;

struct uniform
{
    float alpha = 0.f;  // For animation

    float width  = 0.f; // Resolution
    float height = 0.f;

    float rotate_x_obj = 0.f; // Rotate object
    float rotate_y_obj = 0.f;
    float rotate_z_obj = 0.f;

    float rotate_x_camera = 0.f; // Rotate camera
    float rotate_y_camera = 0.f;
    float rotate_z_camera = 0.f;

    float translate_x_obj = 0.f; // Translate object
    float translate_y_obj = 0.f;
    float translate_z_obj = 0.f;

    float translate_x_camera = 0.f; // Translate camera
    float translate_y_camera = 0.f;
    float translate_z_camera = 0.f;

    float scale_x_obj = 0.f; // Scale object
    float scale_y_obj = 0.f;
    float scale_z_obj = 0.f;
};

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

class engine_checker : engine
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

    uniform uniforms_world;
};