#pragma once
#include "core/types.h"
#include "glad/glad.h"

#include <iostream>

template <class vertex_type>
class vertex_buffer
{
public:
    vertex_buffer(const triangle<vertex_type>* tri, std::size_t n);

    vertex_buffer(const vertex_type* vert, std::size_t n)
        : count(n)
    {
        glGenBuffers(1, &gl_handle);
        GL_CHECK_ERRORS()

        bind();

        GLsizeiptr size_in_bytes =
            static_cast<GLsizeiptr>(n * sizeof(vertex_type));

        glBufferData(GL_ARRAY_BUFFER, size_in_bytes, vert, GL_STATIC_DRAW);
        GL_CHECK_ERRORS()
    }
    // vertex_buffer(const vertex3d_colored* vert, std::size_t n);
    // vertex_buffer(const vertex3d_textured* vert, std::size_t n);
    // vertex_buffer(const vertex3d* vert, std::size_t n);

    ~vertex_buffer()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        GL_CHECK_ERRORS()
        glDeleteBuffers(1, &gl_handle);
        GL_CHECK_ERRORS()
    }

    void bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, gl_handle);
        GL_CHECK_ERRORS()
    }

    uint32_t size() const { return count; }

private:
    uint32_t gl_handle{ 0 };
    uint32_t count{ 0 };
};
