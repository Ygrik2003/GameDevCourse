#include "core/types.h"

#include "glad/glad.h"

class texture_opengl
{
public:
    texture_opengl(const char* path);
    texture_opengl(const void* pixels, const size_t width, const size_t height);
    ~texture_opengl();

    void bind() const;

    uint32_t get_width() const { return width; }
    uint32_t get_height() const { return height; }

private:
    void gen_texture_from_pixels(const void*  pixels,
                                 const size_t width,
                                 const size_t height);

    const char* file_path;
    GLuint      tex_handl = 0;
    uint32_t    width     = 0;
    uint32_t    height    = 0;
};