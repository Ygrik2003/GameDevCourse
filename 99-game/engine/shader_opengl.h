#pragma once
#include "core/types.h"
#include "glad/glad.h"

class shader_opengl
{
public:
    shader_opengl(const char* path_to_vertex, const char* path_to_fragment);
    ~shader_opengl();

    GLuint get_program_id() const;
    void   use() const;

    void reload();

private:
    void load(const char* path, int type);

    GLuint program;

    const char* path_to_vertex;
    const char* path_to_fragment;
};