#include "core/types.h"
#include "glad/glad.h"
#include <cassert>
#include <iostream>

class index_buffer
{
public:
    index_buffer(const uint16_t* i, size_t n)
        : count(n)
    {
        glGenBuffers(1, &gl_handle);
        GL_CHECK_ERRORS();

        bind();

        GLsizeiptr size_in_bytes = n * sizeof(std::uint16_t);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_in_bytes, i, GL_STATIC_DRAW);
        GL_CHECK_ERRORS();
    }
    ~index_buffer()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        GL_CHECK_ERRORS()
        glDeleteBuffers(1, &gl_handle);
        GL_CHECK_ERRORS()
    }
    void bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_handle);
        GL_CHECK_ERRORS();
    }

    std::uint16_t size() const { return count; }

private:
    std::uint32_t gl_handle;
    std::uint16_t count;
};
