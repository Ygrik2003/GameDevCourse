#pragma once
#include "core/config.h"
#include "core/event.h"

#include "KHR/khrplatform.h"
#include "glad/glad.h"

#include <SDL3/SDL.h>
#include <vector>

#define GL_CHECK_ERRORS()                                                      \
    {                                                                          \
        const GLenum err = glGetError();                                       \
        if (err != GL_NO_ERROR)                                                \
        {                                                                      \
            switch (err)                                                       \
            {                                                                  \
                case GL_INVALID_ENUM:                                          \
                    std::cerr << "GL_INVALID_ENUM" << std::endl;               \
                    break;                                                     \
                case GL_INVALID_VALUE:                                         \
                    std::cerr << "GL_INVALID_VALUE" << std::endl;              \
                    break;                                                     \
                case GL_INVALID_OPERATION:                                     \
                    std::cerr << "GL_INVALID_OPERATION" << std::endl;          \
                    break;                                                     \
                case GL_INVALID_FRAMEBUFFER_OPERATION:                         \
                    std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION"            \
                              << std::endl;                                    \
                    break;                                                     \
                case GL_OUT_OF_MEMORY:                                         \
                    std::cerr << "GL_OUT_OF_MEMORY" << std::endl;              \
                    break;                                                     \
            }                                                                  \
            std::cerr << __FILE__ << ':' << __LINE__ << '(' << __FUNCTION__    \
                      << ')' << std::endl;                                     \
            assert(false);                                                     \
        }                                                                      \
    }

struct rgb
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct vertex
{
    float x = 0.f;
    float y = 0.f;
    float z = 0.f;
};

using vertexes = std::vector<vertex>;

struct triangle
{
    triangle()
    {
        v[0] = vertex{};
        v[1] = vertex{};
        v[2] = vertex{};
    }
    triangle(vertex v1, vertex v2, vertex v3)
    {
        v[0] = v1;
        v[1] = v2;
        v[2] = v3;
    }
    vertex v[3];
};

using triangles = std::vector<triangle>;

class engine
{
public:
    virtual ~engine() = default;

    virtual int    initialize(config _config)              = 0;
    virtual void   uninitialize()                          = 0;
    virtual bool   event_keyboard(event&)                  = 0;
    virtual void   render_triangle(const triangle&)        = 0;
    virtual GLuint load_shader(const char* path, int type) = 0;
    virtual void   swap_buffers()                          = 0;
};