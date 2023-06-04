#include "vertex_buffer.h"

// vertex_buffer::vertex_buffer(const triangle<vertex3d>* tri, std::size_t n)
//     : count(static_cast<uint32_t>(n * 3))
// {
//     glGenBuffers(1, &gl_handle);
//     GL_CHECK_ERRORS()

//     bind();

//     GLsizeiptr size_in_bytes =
//         static_cast<GLsizeiptr>(n * 3 * sizeof(vertex3d_colored_textured));

//     glBufferData(
//         GL_ARRAY_BUFFER, size_in_bytes, tri->vertexes.data(),
//         GL_STATIC_DRAW);
//     GL_CHECK_ERRORS()
// }

// vertex_buffer::vertex_buffer(const vertex3d_colored_textured* vert,
// std::size_t n)
//     : count(n)
// {
//     glGenBuffers(1, &gl_handle);
//     GL_CHECK_ERRORS()

//     bind();

//     GLsizeiptr size_in_bytes =
//         static_cast<GLsizeiptr>(n * sizeof(vertex3d_colored_textured));

//     glBufferData(GL_ARRAY_BUFFER, size_in_bytes, vert, GL_STATIC_DRAW);
//     GL_CHECK_ERRORS()
// }

// void vertex_buffer::bind() const
// {
//     glBindBuffer(GL_ARRAY_BUFFER, gl_handle);
//     GL_CHECK_ERRORS()
// }
