#include "engine.h"
#include "core/picopng.hxx"

#include "objects/mesh.h"
#include <filesystem>
#include <fstream>
#include <stdexcept>

#ifndef _WIN32
void APIENTRY gl_debug_output(GLenum        source,
                              GLenum        type,
                              GLuint        id,
                              GLenum        severity,
                              GLsizei       length,
                              const GLchar* message,
                              const void*   userParam)
{

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:
            std::cout << "Source: API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            std::cout << "Source: Window System";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            std::cout << "Source: Shader Compiler";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            std::cout << "Source: Third Party";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            std::cout << "Source: Application";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            std::cout << "Source: Other";
            break;
    }
    std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:
            std::cout << "Type: Error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            std::cout << "Type: Deprecated Behaviour";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            std::cout << "Type: Undefined Behaviour";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            std::cout << "Type: Portability";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            std::cout << "Type: Performance";
            break;
        case GL_DEBUG_TYPE_MARKER:
            std::cout << "Type: Marker";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            std::cout << "Type: Push Group";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            std::cout << "Type: Pop Group";
            break;
        case GL_DEBUG_TYPE_OTHER:
            std::cout << "Type: Other";
            break;
    }
    std::cout << std::endl;

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            std::cout << "Severity: high";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            std::cout << "Severity: medium";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            std::cout << "Severity: low";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            std::cout << "Severity: notification";
            break;
    }
    std::cout << std::endl;
    std::cout << std::endl;
}
#endif

template <class vertex_type>
void bind_vertexes(const triangle<vertex_type>& tr)
{
    glEnableVertexAttribArray(0);
    GL_CHECK_ERRORS()
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(tr.vertexes[0]),
                          reinterpret_cast<GLvoid*>(0));
    GL_CHECK_ERRORS()
}

template <class vertex_type>
void bind_normals(const triangle<vertex_type>& tr)
{
    glEnableVertexAttribArray(1);
    GL_CHECK_ERRORS()
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(tr.vertexes[0]),
                          reinterpret_cast<GLvoid*>(3 * sizeof(float)));
    GL_CHECK_ERRORS()
}

void bind_texture_coords(const triangle<vertex_textured>& tr)
{
    glEnableVertexAttribArray(2);
    GL_CHECK_ERRORS()
    glVertexAttribPointer(2,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(tr.vertexes[0]),
                          reinterpret_cast<GLvoid*>(sizeof(vertex)));
    GL_CHECK_ERRORS()
}

void bind_texture_coords(const triangle<vertex_colored_textured>& tr)
{
    glEnableVertexAttribArray(2);
    GL_CHECK_ERRORS()
    glVertexAttribPointer(2,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(tr.vertexes[0]),
                          reinterpret_cast<GLvoid*>(sizeof(vertex_colored)));
    GL_CHECK_ERRORS()
}

template <class vertex_colored_type>
void bind_colors(const triangle<vertex_colored_type>& tr)
{
    glEnableVertexAttribArray(3);
    GL_CHECK_ERRORS()
    glVertexAttribPointer(3,
                          4,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(tr.vertexes[0]),
                          reinterpret_cast<GLvoid*>(sizeof(vertex)));
    GL_CHECK_ERRORS()
}

void* load_gl_func(const char* name)
{
    SDL_FunctionPointer gl_pointer = SDL_GL_GetProcAddress(name);
    if (gl_pointer == nullptr)
    {
        throw std::runtime_error(std::string("can't load GL function: ") +
                                 name);
    }
    return reinterpret_cast<void*>(gl_pointer);
}

int engine_opengl::initialize(config cfg)
{

    _config = cfg;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

    if (_config.is_full_sreen)
    {
        window = SDL_CreateWindow("Sphere Render",
                                  _config.width,
                                  _config.height,
                                  SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
        int w, h;
        SDL_GetWindowSize(window, &w, &h);
        _config.width  = w;
        _config.height = h;
    }
    else
    {
        window = SDL_CreateWindow(
            "Sphere Render", _config.width, _config.height, SDL_WINDOW_OPENGL);
    }

    if (window == nullptr)
    {
        std::cerr << SDL_GetError();
        SDL_Quit();
        return 0;
    }

    int gl_major_v = 3;
    int gl_minor_v = 2;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, gl_major_v);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, gl_minor_v);

    gl_context = SDL_GL_CreateContext(window);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    if (gl_context == nullptr)
    {
        std::cerr << SDL_GetError();
        SDL_Quit();
        return 0;
    }

    int result = SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &gl_major_v);
    assert(result == 0);
    result = SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &gl_minor_v);
    assert(result == 0);

    std::clog << "OpenGL " << gl_major_v << '.' << gl_minor_v << '\n';

    if (gladLoadGLES2Loader(load_gl_func) == 0)
    {
        std::cerr << "cant init glad" << std::endl;
        SDL_Quit();
        return 0;
    }

#ifndef _WIN32
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(gl_debug_output, nullptr);
    glDebugMessageControl(
        GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif

    GL_CHECK_ERRORS()

    shader = new shader_opengl(_config.shader_vertex, _config.shader_fragment);
    shader->use();

    GLuint vertex_buffer = 0;
    glGenBuffers(1, &vertex_buffer);
    GL_CHECK_ERRORS()
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    GL_CHECK_ERRORS()
    GLuint vertex_array_object = 0;
    glGenVertexArrays(1, &vertex_array_object);
    GL_CHECK_ERRORS()
    glBindVertexArray(vertex_array_object);
    GL_CHECK_ERRORS()

    glBindAttribLocation(shader->get_program_id(), 0, "i_position");
    GL_CHECK_ERRORS()

    uniforms = glGetUniformLocation(shader->get_program_id(), "u_uniforms");
    GL_CHECK_ERRORS()
    uniform_texture =
        glGetUniformLocation(shader->get_program_id(), "u_texture");
    GL_CHECK_ERRORS()

    glEnable(GL_BLEND);
    GL_CHECK_ERRORS()
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GL_CHECK_ERRORS()

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return 1;
}

void engine_opengl::uninitialize()
{
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool engine_opengl::event_keyboard(event& e)
{
    e.clear();
    bool      is_event = false;
    SDL_Event sdl_event;

    if (SDL_PollEvent(&sdl_event))
    {
        if (sdl_event.type == SDL_EVENT_QUIT)
        {
            e.action.quit = true;
            is_event      = true;
        }
        else if (sdl_event.type == SDL_EVENT_KEY_DOWN)
        {
            // clang-format off
            if (sdl_event.key.keysym.sym == SDLK_w) e.keyboard.up_clicked        = 1;
            if (sdl_event.key.keysym.sym == SDLK_s) e.keyboard.down_clicked      = 1;
            if (sdl_event.key.keysym.sym == SDLK_a) e.keyboard.left_clicked      = 1;
            if (sdl_event.key.keysym.sym == SDLK_d) e.keyboard.right_clicked     = 1;
            if (sdl_event.key.keysym.sym == SDLK_SPACE) e.keyboard.space_clicked = 1;
            // clang-format on
            is_event = true;
        }
        else if (sdl_event.type == SDL_EVENT_KEY_UP)
        {
            // clang-format off
            if (sdl_event.key.keysym.sym == SDLK_w) e.keyboard.up_released        = 1;
            if (sdl_event.key.keysym.sym == SDLK_s) e.keyboard.down_released      = 1;
            if (sdl_event.key.keysym.sym == SDLK_a) e.keyboard.left_released      = 1;
            if (sdl_event.key.keysym.sym == SDLK_d) e.keyboard.right_released     = 1;
            if (sdl_event.key.keysym.sym == SDLK_SPACE) e.keyboard.space_released = 1;
            // clang-format on
            is_event = true;
        }
        else if (sdl_event.type == SDL_EVENT_MOUSE_MOTION)
        {
            e.motion.x = sdl_event.motion.xrel;
            e.motion.y = sdl_event.motion.yrel;
            is_event   = true;
        }
    }
    return is_event;
}

void engine_opengl::render_triangle(const triangle<vertex>& tr)
{
    reload_uniform();

    glBufferData(GL_ARRAY_BUFFER, sizeof(tr), &tr, GL_STATIC_DRAW);
    GL_CHECK_ERRORS()

    bind_vertexes(tr);
    bind_normals(tr);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void engine_opengl::render_triangle(const triangle<vertex_colored>& tr)
{
    reload_uniform();

    glBufferData(GL_ARRAY_BUFFER, sizeof(tr), &tr, GL_STATIC_DRAW);
    GL_CHECK_ERRORS()

    bind_vertexes(tr);
    bind_normals(tr);
    bind_colors(tr);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void engine_opengl::render_triangle(const triangle<vertex_textured>& tr)
{
    reload_uniform();

    glBufferData(GL_ARRAY_BUFFER, sizeof(tr), &tr, GL_STATIC_DRAW);
    GL_CHECK_ERRORS()

    bind_vertexes(tr);
    bind_normals(tr);
    bind_texture_coords(tr);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void engine_opengl::render_triangle(const triangle<vertex_colored_textured>& tr)
{
    reload_uniform();

    glBufferData(GL_ARRAY_BUFFER, sizeof(tr), &tr, GL_STATIC_DRAW);
    GL_CHECK_ERRORS()

    bind_vertexes(tr);
    bind_normals(tr);
    bind_texture_coords(tr);
    bind_colors(tr);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void engine_opengl::swap_buffers()
{
    SDL_GL_SwapWindow(window);

    glClearColor(1.f, 1.f, 1.f, 1.f);
    GL_CHECK_ERRORS()

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GL_CHECK_ERRORS()
}

void engine_opengl::load_texture(size_t index, const char* path)
{
    glActiveTexture(GL_TEXTURE0 + index);
    GL_CHECK_ERRORS()
    glUniform1i(uniform_texture, index);

    if (!std::filesystem::exists(path))
        throw std::runtime_error("No file");

    std::vector<std::byte> png_file_in_memory;
    std::ifstream          ifs(path, std::ios_base::binary);

    size_t pos_in_file = std::filesystem::file_size(path);
    png_file_in_memory.resize(pos_in_file);

    ifs.read(reinterpret_cast<char*>(png_file_in_memory.data()),
             static_cast<std::streamsize>(png_file_in_memory.size()));

    std::vector<std::byte> image;
    unsigned long          w = 0;
    unsigned long          h = 0;

    decodePNG(image,
              w,
              h,
              png_file_in_memory.data(),
              png_file_in_memory.size(),
              false);

    GLuint tex_handl = 0;
    glGenTextures(1, &tex_handl);
    GL_CHECK_ERRORS()
    glBindTexture(GL_TEXTURE_2D, tex_handl);
    GL_CHECK_ERRORS()

    GLint mipmap_level = 0;
    GLint border       = 0;
    // clang-format off
    glTexImage2D(GL_TEXTURE_2D,
                 mipmap_level,
                 GL_RGB,
                 static_cast<GLsizei>(w),
                 static_cast<GLsizei>(h),
                 border,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 image.data());
    // clang-format on
    GL_CHECK_ERRORS()

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    GL_CHECK_ERRORS()
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    GL_CHECK_ERRORS()
}

void engine_opengl::set_texture(size_t index)
{
    glActiveTexture(GL_TEXTURE0 + index);
    glUniform1i(uniform_texture, index);
}

figure engine_opengl::load_object(const char* path)
{
    std::vector<vertex_textured> vertexes;
    std::vector<uint32_t>        indexes;
    return mesh(vertexes, indexes);
}

void engine_opengl::create_shadow_map()
{
    glGenFramebuffers(1, &obj_depth_map);

    glGenTextures(1, &texture_depth_map);
    glBindTexture(GL_TEXTURE_2D, texture_depth_map);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_DEPTH_COMPONENT,
                 _config.width,
                 _config.height,
                 0,
                 GL_DEPTH_COMPONENT,
                 GL_FLOAT,
                 NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, obj_depth_map);
    glFramebufferTexture2D(GL_FRAMEBUFFER,
                           GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D,
                           texture_depth_map,
                           0);
    // glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void engine_opengl::set_uniform(uniform& uni)
{
    this->uniforms_world = &uni;
}

void engine_opengl::reload_uniform()
{
    glUniform1f(
        glGetUniformLocation(shader->get_program_id(), "u_uniforms.width"),
        uniforms_world->width);
    glUniform1f(
        glGetUniformLocation(shader->get_program_id(), "u_uniforms.height"),
        uniforms_world->height);

    glUniform1f(glGetUniformLocation(shader->get_program_id(),
                                     "u_uniforms.rotate_alpha_obj"),
                *uniforms_world->rotate_alpha_obj);
    glUniform1f(glGetUniformLocation(shader->get_program_id(),
                                     "u_uniforms.rotate_beta_obj"),
                *uniforms_world->rotate_beta_obj);
    glUniform1f(glGetUniformLocation(shader->get_program_id(),
                                     "u_uniforms.rotate_gamma_obj"),
                *uniforms_world->rotate_gamma_obj);

    glUniform1f(glGetUniformLocation(shader->get_program_id(),
                                     "u_uniforms.rotate_alpha_camera"),
                *uniforms_world->rotate_alpha_camera);
    glUniform1f(glGetUniformLocation(shader->get_program_id(),
                                     "u_uniforms.rotate_beta_camera"),
                *uniforms_world->rotate_beta_camera);
    glUniform1f(glGetUniformLocation(shader->get_program_id(),
                                     "u_uniforms.rotate_gamma_camera"),
                *uniforms_world->rotate_gamma_camera);

    glUniform1f(glGetUniformLocation(shader->get_program_id(),
                                     "u_uniforms.translate_x_obj"),
                *uniforms_world->translate_x_obj);
    glUniform1f(glGetUniformLocation(shader->get_program_id(),
                                     "u_uniforms.translate_y_obj"),
                *uniforms_world->translate_y_obj);
    glUniform1f(glGetUniformLocation(shader->get_program_id(),
                                     "u_uniforms.translate_z_obj"),
                *uniforms_world->translate_z_obj);

    glUniform1f(glGetUniformLocation(shader->get_program_id(),
                                     "u_uniforms.translate_x_camera"),
                *uniforms_world->translate_x_camera);
    glUniform1f(glGetUniformLocation(shader->get_program_id(),
                                     "u_uniforms.translate_y_camera"),
                *uniforms_world->translate_y_camera);
    glUniform1f(glGetUniformLocation(shader->get_program_id(),
                                     "u_uniforms.translate_z_camera"),
                *uniforms_world->translate_z_camera);

    glUniform1f(glGetUniformLocation(shader->get_program_id(),
                                     "u_uniforms.scale_x_obj"),
                *uniforms_world->scale_x_obj);
    glUniform1f(glGetUniformLocation(shader->get_program_id(),
                                     "u_uniforms.scale_y_obj"),
                *uniforms_world->scale_y_obj);
    glUniform1f(glGetUniformLocation(shader->get_program_id(),
                                     "u_uniforms.scale_z_obj"),
                *uniforms_world->scale_z_obj);
}
