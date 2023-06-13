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