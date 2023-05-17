#include "engine.h"
#include "core/picopng.hxx"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

#define GL_DEBUG_OUTPUT_USE

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
PFNGLUNIFORM3FPROC               glUniform3f               = nullptr;
PFNGLUNIFORM1IPROC               glUniform1i               = nullptr;
PFNGLUNIFORM1FVPROC              glUniform1fv              = nullptr;
PFNGLBUFFERDATAPROC              glBufferData              = nullptr;

#ifdef GL_DEBUG_OUTPUT_USE
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

template <typename T>
static void load_gl_func(const char* func_name, T& result)
{
    SDL_FunctionPointer gl_pointer = SDL_GL_GetProcAddress(func_name);
    if (gl_pointer == nullptr)
    {
        throw std::runtime_error(std::string("can't load GL function") +
                                 func_name);
    }
    result = reinterpret_cast<T>(gl_pointer);
}

int engine_checkers::initialize(config cfg)
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

    load_gl_func("glCreateShader", glCreateShader);
    load_gl_func("glShaderSource", glShaderSource);
    load_gl_func("glCompileShader", glCompileShader);
    load_gl_func("glGetShaderiv", glGetShaderiv);
    load_gl_func("glGetShaderInfoLog", glGetShaderInfoLog);
    load_gl_func("glDeleteShader", glDeleteShader);
    load_gl_func("glCreateProgram", glCreateProgram);
    load_gl_func("glAttachShader", glAttachShader);
    load_gl_func("glBindAttribLocation", glBindAttribLocation);
    load_gl_func("glLinkProgram", glLinkProgram);
    load_gl_func("glGetProgramiv", glGetProgramiv);
    load_gl_func("glGetProgramInfoLog", glGetProgramInfoLog);
    load_gl_func("glDeleteProgram", glDeleteProgram);
    load_gl_func("glUseProgram", glUseProgram);
    load_gl_func("glVertexAttribPointer", glVertexAttribPointer);
    load_gl_func("glEnableVertexAttribArray", glEnableVertexAttribArray);
    load_gl_func("glGenBuffers", glGenBuffers);
    load_gl_func("glBindBuffer", glBindBuffer);
    load_gl_func("glGenVertexArrays", glGenVertexArrays);
    load_gl_func("glBindVertexArray", glBindVertexArray);
    load_gl_func("glGetUniformLocation", glGetUniformLocation);
    load_gl_func("glUniform1f", glUniform1f);
    load_gl_func("glUniform3f", glUniform3f);
    load_gl_func("glUniform1i", glUniform1i);
    load_gl_func("glUniform1fv", glUniform1fv);
    load_gl_func("glBufferData", glBufferData);

#ifdef GL_DEBUG_OUTPUT_USE
    load_gl_func("glDebugMessageCallback", glDebugMessageCallback);
    load_gl_func("glDebugMessageControl", glDebugMessageControl);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(gl_debug_output, nullptr);
    glDebugMessageControl(
        GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif

    shader_program = glCreateProgram();
    GL_CHECK_ERRORS()

    load_shader(_config.shader_vertex, GL_VERTEX_SHADER);
    load_shader(_config.shader_fragment, GL_FRAGMENT_SHADER);

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

    glBindAttribLocation(shader_program, 0, "i_position");
    GL_CHECK_ERRORS()

    glLinkProgram(shader_program);
    GL_CHECK_ERRORS()

    glUseProgram(shader_program);
    GL_CHECK_ERRORS()

    uniforms = glGetUniformLocation(shader_program, "u_uniforms");
    GL_CHECK_ERRORS()
    uniform_texture = glGetUniformLocation(shader_program, "u_texture");
    GL_CHECK_ERRORS()

    glEnable(GL_BLEND);
    GL_CHECK_ERRORS()
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GL_CHECK_ERRORS()

    glEnable(GL_DEPTH_TEST);

    return 1;
}

void engine_checkers::uninitialize()
{
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool engine_checkers::event_keyboard(event& e)
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

void engine_checkers::render_triangle(const triangle<vertex_textured>& tr)
{

    glUniform1f(glGetUniformLocation(shader_program, "u_uniforms.width"),
                uniforms_world->width);
    glUniform1f(glGetUniformLocation(shader_program, "u_uniforms.height"),
                uniforms_world->height);

    glUniform1f(
        glGetUniformLocation(shader_program, "u_uniforms.rotate_alpha_obj"),
        *uniforms_world->rotate_alpha_obj);
    glUniform1f(
        glGetUniformLocation(shader_program, "u_uniforms.rotate_beta_obj"),
        *uniforms_world->rotate_beta_obj);
    glUniform1f(
        glGetUniformLocation(shader_program, "u_uniforms.rotate_gamma_obj"),
        *uniforms_world->rotate_gamma_obj);

    glUniform1f(
        glGetUniformLocation(shader_program, "u_uniforms.rotate_alpha_camera"),
        *uniforms_world->rotate_alpha_camera);
    glUniform1f(
        glGetUniformLocation(shader_program, "u_uniforms.rotate_beta_camera"),
        *uniforms_world->rotate_beta_camera);
    glUniform1f(
        glGetUniformLocation(shader_program, "u_uniforms.rotate_gamma_camera"),
        *uniforms_world->rotate_gamma_camera);

    glUniform1f(
        glGetUniformLocation(shader_program, "u_uniforms.translate_x_obj"),
        *uniforms_world->translate_x_obj);
    glUniform1f(
        glGetUniformLocation(shader_program, "u_uniforms.translate_y_obj"),
        *uniforms_world->translate_y_obj);
    glUniform1f(
        glGetUniformLocation(shader_program, "u_uniforms.translate_z_obj"),
        *uniforms_world->translate_z_obj);

    glUniform1f(
        glGetUniformLocation(shader_program, "u_uniforms.translate_x_camera"),
        *uniforms_world->translate_x_camera);
    glUniform1f(
        glGetUniformLocation(shader_program, "u_uniforms.translate_y_camera"),
        *uniforms_world->translate_y_camera);
    glUniform1f(
        glGetUniformLocation(shader_program, "u_uniforms.translate_z_camera"),
        *uniforms_world->translate_z_camera);

    glUniform1f(glGetUniformLocation(shader_program, "u_uniforms.scale_x_obj"),
                *uniforms_world->scale_x_obj);
    glUniform1f(glGetUniformLocation(shader_program, "u_uniforms.scale_y_obj"),
                *uniforms_world->scale_y_obj);
    glUniform1f(glGetUniformLocation(shader_program, "u_uniforms.scale_z_obj"),
                *uniforms_world->scale_z_obj);

    glUniform3f(glGetUniformLocation(shader_program, "u_normal"),
                tr.normal.x,
                tr.normal.y,
                tr.normal.z);

    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(triangle<vertex_textured>),
                 &tr,
                 GL_STATIC_DRAW);
    GL_CHECK_ERRORS()

    glEnableVertexAttribArray(0);
    GL_CHECK_ERRORS()
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(vertex_textured),
                          reinterpret_cast<GLvoid*>(0));
    GL_CHECK_ERRORS()

    glEnableVertexAttribArray(1);
    GL_CHECK_ERRORS()
    glVertexAttribPointer(1,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(vertex_textured),
                          reinterpret_cast<GLvoid*>(3 * sizeof(float)));
    GL_CHECK_ERRORS()

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void engine_checkers::swap_buffers()
{
    SDL_GL_SwapWindow(window);

    glClearColor(1.f, 1.f, 1.f, 1.f);
    GL_CHECK_ERRORS()

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GL_CHECK_ERRORS()
}

void engine_checkers::load_texture(size_t index, const char* path)
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

void engine_checkers::set_texture(size_t index)
{
    glActiveTexture(GL_TEXTURE0 + index);
    glUniform1i(uniform_texture, index);
}

void engine_checkers::set_uniform(uniform& uni)
{
    this->uniforms_world = &uni;
}

void engine_checkers::reload_shader(const char* path_to_vertex,
                                    const char* path_to_fragment)
{
    glDeleteProgram(shader_program);
    GL_CHECK_ERRORS()

    shader_program = glCreateProgram();
    GL_CHECK_ERRORS()

    load_shader(path_to_vertex, GL_VERTEX_SHADER);
    load_shader(path_to_fragment, GL_FRAGMENT_SHADER);

    glLinkProgram(shader_program);
    GL_CHECK_ERRORS()

    glUseProgram(shader_program);
    GL_CHECK_ERRORS()
}

void engine_checkers::load_shader(const char* path, int type)
{
    if (!std::filesystem::exists(path))
        throw std::runtime_error("No file");

    GLuint handle = glCreateShader(type);
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
    glShaderSource(handle, 1, &c_buffer, nullptr);
    GL_CHECK_ERRORS()

    glCompileShader(handle);
    GL_CHECK_ERRORS()

    GLint result = 0;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &result);
    GL_CHECK_ERRORS()
    if (result == 0)
    {
        glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &result);
        GL_CHECK_ERRORS()

        std::vector<char> log(static_cast<size_t>(result));
        glGetShaderInfoLog(handle, result, nullptr, log.data());
        GL_CHECK_ERRORS()

        glDeleteShader(handle);
        GL_CHECK_ERRORS()

        throw std::runtime_error(log.data());
    }
    glAttachShader(shader_program, handle);
    GL_CHECK_ERRORS()

    glDeleteShader(handle);
    GL_CHECK_ERRORS()

    // if (type == GL_VERTEX_SHADER)
    //     shader_vertex = handle;
    // else if (type == GL_FRAGMENT_SHADER)
    //     shader_fragment_chessboard_cells = handle;
}
