#include "sphere_engine.h"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>

#define GL_DEBUG_OUTPUT_USE

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

sphere_engine::sphere_engine() {}

int sphere_engine::initialize(config _config)
{
    this->_config = _config;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

    window = SDL_CreateWindow(
        "Sphere Render", 100, 100, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);

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

#ifdef GL_DEBUG_OUTPUT_USE
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(gl_debug_output, nullptr);
    glDebugMessageControl(
        GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif

    GLuint vertex_shader = load_shader(_config.vertex_shader, GL_VERTEX_SHADER);

    GLuint fragment_shader =
        load_shader(_config.fragment_shader, GL_FRAGMENT_SHADER);

    handle_program = glCreateProgram();
    GL_CHECK_ERRORS()

    if (handle_program == 0)
    {
        std::cerr << "cant create program" << std::endl;
        SDL_Quit();
        return 0;
    }

    glAttachShader(handle_program, vertex_shader);
    GL_CHECK_ERRORS()

    glAttachShader(handle_program, fragment_shader);
    GL_CHECK_ERRORS()

    glBindAttribLocation(handle_program, 0, "a_position");
    GL_CHECK_ERRORS()

    glLinkProgram(handle_program);
    GL_CHECK_ERRORS()

    uniform_tr_obj_rotate =
        glGetUniformLocation(handle_program, "tr_obj.rotate");
    GL_CHECK_ERRORS()
    uniform_tr_obj_scale = glGetUniformLocation(handle_program, "tr_obj.scale");
    GL_CHECK_ERRORS()
    uniform_tr_obj_translate =
        glGetUniformLocation(handle_program, "tr_obj.translate");
    GL_CHECK_ERRORS()
    uniform_tr_cam_rotate =
        glGetUniformLocation(handle_program, "tr_cam.rotate");
    GL_CHECK_ERRORS()
    uniform_tr_cam_scale = glGetUniformLocation(handle_program, "tr_cam.scale");
    GL_CHECK_ERRORS()
    uniform_tr_cam_translate =
        glGetUniformLocation(handle_program, "tr_cam.translate");
    GL_CHECK_ERRORS()
    uniform_tr_cam_projection =
        glGetUniformLocation(handle_program, "tr_cam.projection");
    GL_CHECK_ERRORS()

    glGetProgramiv(handle_program, GL_LINK_STATUS, &result);
    GL_CHECK_ERRORS()
    if (result == 0)
    {
        glGetProgramiv(handle_program, GL_INFO_LOG_LENGTH, &result);
        GL_CHECK_ERRORS()

        std::vector<char> log(static_cast<size_t>(result));
        glGetProgramInfoLog(handle_program, result, nullptr, log.data());
        GL_CHECK_ERRORS()

        glDeleteShader(handle_program);
        GL_CHECK_ERRORS()

        throw std::runtime_error(log.data());
    }

    glUseProgram(handle_program);
    GL_CHECK_ERRORS()

    glEnable(GL_DEPTH_TEST);

    return 1;
}

bool sphere_engine::event_keyboard(event& e)
{
    e.clear();
    SDL_Event sdl_event;

    if (SDL_PollEvent(&sdl_event))
    {
        if (sdl_event.type == SDL_EVENT_QUIT)
        {
            e.action.quit = true;
            return true;
        }
        else if (sdl_event.type == SDL_EVENT_KEY_DOWN)
        {
            // clang-format off
            if (sdl_event.key.keysym.sym == SDLK_w) e.keyboard.up_clicked    = 1;
            if (sdl_event.key.keysym.sym == SDLK_s) e.keyboard.down_clicked  = 1;
            if (sdl_event.key.keysym.sym == SDLK_a) e.keyboard.left_clicked  = 1;
            if (sdl_event.key.keysym.sym == SDLK_d) e.keyboard.right_clicked = 1;
            // clang-format on
            return true;
        }
        else if (sdl_event.type == SDL_EVENT_KEY_UP)
        {
            // clang-format off
            if (sdl_event.key.keysym.sym == SDLK_w) e.keyboard.up_released    = 1;
            if (sdl_event.key.keysym.sym == SDLK_s) e.keyboard.down_released  = 1;
            if (sdl_event.key.keysym.sym == SDLK_a) e.keyboard.left_released  = 1;
            if (sdl_event.key.keysym.sym == SDLK_d) e.keyboard.right_released = 1;
            // clang-format on
            return true;
        }
        else if (sdl_event.type == SDL_EVENT_MOUSE_MOTION)
        {

            e.motion.x = sdl_event.motion.x;
            e.motion.y = sdl_event.motion.y;
            return true;
        }
    }
    return false;
}

void sphere_engine::uninitialize()
{
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void sphere_engine::update() {}

void sphere_engine::render(const triangles& trs) {}

GLuint sphere_engine::load_shader(const char* path, int type)
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

    return handle;
}

void sphere_engine::render_triangle(const triangle&        tr,
                                    transformation_object& uniforms_1,
                                    transformation_camera& uniforms_2)
{
    glUniformMatrix3fv(
        uniform_tr_obj_rotate, 1, GL_FALSE, &uniforms_1.rotate[0][0]);
    GL_CHECK_ERRORS()
    glUniform3fv(uniform_tr_obj_scale, 1, uniforms_1.scale);
    GL_CHECK_ERRORS()
    glUniform3fv(uniform_tr_obj_translate, 1, uniforms_1.translate);
    GL_CHECK_ERRORS()

    glUniformMatrix4fv(
        uniform_tr_cam_rotate, 1, GL_FALSE, &uniforms_2.rotate[0][0]);
    GL_CHECK_ERRORS()
    glUniformMatrix4fv(
        uniform_tr_cam_scale, 1, GL_FALSE, &uniforms_2.scale[0][0]);
    GL_CHECK_ERRORS()
    glUniformMatrix4fv(
        uniform_tr_cam_translate, 1, GL_FALSE, &uniforms_2.translate[0][0]);
    GL_CHECK_ERRORS()
    glUniformMatrix4fv(
        uniform_tr_cam_projection, 1, GL_FALSE, &uniforms_2.projection[0][0]);
    GL_CHECK_ERRORS()

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), &tr.v[0]);
    GL_CHECK_ERRORS()

    glEnableVertexAttribArray(0);
    GL_CHECK_ERRORS()

    glDrawArrays(GL_TRIANGLES, 0, 3);
    GL_CHECK_ERRORS()
}

void sphere_engine::swap_buffers()
{
    SDL_GL_SwapWindow(window);

    glClearColor(1.f, 1.f, 1.f, 1.f);
    GL_CHECK_ERRORS()

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GL_CHECK_ERRORS()
}