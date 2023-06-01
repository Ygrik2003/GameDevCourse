#include "shader_opengl.h"
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

shader_opengl::shader_opengl(const char* path_to_vertex,
                             const char* path_to_fragment)
{
    this->path_to_vertex   = path_to_vertex;
    this->path_to_fragment = path_to_fragment;

    program = glCreateProgram();
    GL_CHECK_ERRORS()

    load(path_to_vertex, GL_VERTEX_SHADER);
    load(path_to_fragment, GL_FRAGMENT_SHADER);

    glLinkProgram(program);
    GL_CHECK_ERRORS()

    use();
}

shader_opengl::~shader_opengl()
{
    glDeleteProgram(program);
}

GLuint shader_opengl::get_program_id() const
{
    return program;
}

void shader_opengl::use() const
{
    glUseProgram(program);
    GL_CHECK_ERRORS();
}

void shader_opengl::reload()
{
    glDeleteProgram(program);
    GL_CHECK_ERRORS()

    program = glCreateProgram();
    GL_CHECK_ERRORS()

    load(path_to_vertex, GL_VERTEX_SHADER);
    load(path_to_fragment, GL_FRAGMENT_SHADER);

    glLinkProgram(program);
    GL_CHECK_ERRORS()
}

void shader_opengl::load(const char* path, int type)
{
    if (!std::filesystem::exists(path))
        throw std::runtime_error("No file: " + std::string(path));

    GLuint shader = glCreateShader(type);
    GL_CHECK_ERRORS()

    std::string   buffer;
    std::ifstream file(path);

    file.seekg(0, std::ios::end);
    buffer.reserve(file.tellg());

    file.seekg(0, std::ios::beg);
    buffer.assign(std::istreambuf_iterator<char>(file),
                  std::istreambuf_iterator<char>());

    const char* c_buffer = buffer.data();
    file.close();
    glShaderSource(shader, 1, &c_buffer, nullptr);
    GL_CHECK_ERRORS()

    glCompileShader(shader);
    GL_CHECK_ERRORS()

    GLint result = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    GL_CHECK_ERRORS()
    if (result == 0)
    {
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &result);
        GL_CHECK_ERRORS()

        std::vector<char> log(static_cast<size_t>(result));
        glGetShaderInfoLog(shader, result, nullptr, log.data());
        GL_CHECK_ERRORS()

        glDeleteShader(shader);
        GL_CHECK_ERRORS()

        throw std::runtime_error(log.data());
    }
    glAttachShader(program, shader);
    GL_CHECK_ERRORS()

    glDeleteShader(shader);
    GL_CHECK_ERRORS()
}
