#include "engine.h"

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
void bind_vertexes()
{
    glEnableVertexAttribArray(0);
    GL_CHECK_ERRORS()
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(vertex_type),
        reinterpret_cast<GLvoid*>(vertex_type::OFFSET_POSITION));
    GL_CHECK_ERRORS()
}

template <class vertex_type>
void bind_normal()
{
    glEnableVertexAttribArray(1);
    GL_CHECK_ERRORS()
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(vertex_type),
        reinterpret_cast<GLvoid*>(vertex_type::OFFSET_NORMAL));
    GL_CHECK_ERRORS()
}

template <class vertex_type>
void bind_texture_coords()
{
    glEnableVertexAttribArray(2);
    GL_CHECK_ERRORS()
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(vertex_type),
        reinterpret_cast<GLvoid*>(vertex_type::OFFSET_TEXTURE));
    GL_CHECK_ERRORS()
}

template <class vertex_type>
void bind_colors()
{
    glEnableVertexAttribArray(3);
    GL_CHECK_ERRORS()
    glVertexAttribPointer(3,
                          4,
                          GL_UNSIGNED_BYTE,
                          GL_TRUE,
                          sizeof(vertex_type),
                          reinterpret_cast<GLvoid*>(vertex_type::OFFSET_COLOR));
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

static float          g_Time            = 0.0;
static bool           g_MousePressed[3] = { false, false, false };
static float          g_MouseWheel      = 0.0f;
static shader_opengl* g_imgui_shader    = nullptr;

void ImGui_ImplSdlGL3_RenderDrawLists(engine* eng, ImDrawData* draw_data)
{
    // Avoid rendering when minimized, scale coordinates for retina displays
    // (screen coordinates != framebuffer coordinates)
    ImGuiIO& io        = ImGui::GetIO();
    int      fb_width  = int(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    int      fb_height = int(io.DisplaySize.y * io.DisplayFramebufferScale.y);
    if (fb_width == 0 || fb_height == 0)
    {
        return;
    }
    draw_data->ScaleClipRects(io.DisplayFramebufferScale);

    texture_opengl* texture =
        reinterpret_cast<texture_opengl*>(io.Fonts->TexID);

    g_imgui_shader->use();

    unsigned int texture_unit = 0;
    texture->bind();

    g_imgui_shader->set_uniform1("u_texture",
                                 static_cast<int>(0 + texture_unit));

    glDisable(GL_DEPTH_TEST);
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list          = draw_data->CmdLists[n];
        const ImDrawIdx*  idx_buffer_offset = nullptr;

        const vertex2d_colored_textured* vertex_data =
            reinterpret_cast<const vertex2d_colored_textured*>(
                cmd_list->VtxBuffer.Data);
        size_t vert_count = static_cast<size_t>(cmd_list->VtxBuffer.size());

        vertex_buffer<vertex2d_colored_textured>* vertex_buff =
            new vertex_buffer(vertex_data, vert_count);

        const std::uint16_t* indexes = cmd_list->IdxBuffer.Data;
        size_t index_count = static_cast<size_t>(cmd_list->IdxBuffer.size());

        index_buffer* index_buff = new index_buffer(indexes, index_count);

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];

            texture_opengl* tex =
                reinterpret_cast<texture_opengl*>(pcmd->TextureId);
            eng->render_triangles(vertex_buff,
                                  index_buff,
                                  tex,
                                  idx_buffer_offset,
                                  pcmd->ElemCount);

            idx_buffer_offset += pcmd->ElemCount;
        } // end for cmd_i
        delete vertex_buff;
        delete index_buff;
    } // end for n
    glEnable(GL_DEPTH_TEST);
}

static const char* ImGui_ImplSdlGL3_GetClipboardText(void*)
{
    return SDL_GetClipboardText();
}

static void ImGui_ImplSdlGL3_SetClipboardText(void*, const char* text)
{
    SDL_SetClipboardText(text);
}

int engine_opengl::initialize(config& cfg)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) > 0)
    {
        std::runtime_error(std::string("Error in Init SDL3: ") +
                           SDL_GetError());
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

    if (cfg.is_full_sreen)
    {
        window = SDL_CreateWindow("Tetris3D",
                                  cfg.width,
                                  cfg.height,
                                  SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
    }
    else
    {
        window = SDL_CreateWindow(
            "Sphere Render", cfg.width, cfg.height, SDL_WINDOW_OPENGL);
    }
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    cfg.width  = w;
    cfg.height = h;

    _config = cfg;

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
    // SDL_SetRelativeMouseMode(SDL_TRUE);
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
    GL_CHECK_ERRORS()
#endif


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

    glEnable(GL_BLEND);
    GL_CHECK_ERRORS()
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GL_CHECK_ERRORS()

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (!ImGui_ImplSdlGL3_Init(window))
    {
        std::runtime_error("error: failed to init ImGui");
    }

    ImGui_ImplSdlGL3_NewFrame(window);

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
    bool      is_event = false;
    SDL_Event sdl_event;

    while (SDL_PollEvent(&sdl_event))
    {
        ImGui_ImplSdlGL3_ProcessEvent(&sdl_event);
        if (sdl_event.type == SDL_EVENT_QUIT)
        {
            e.action.quit = true;
            is_event      = true;
        }
        else if (sdl_event.type == SDL_EVENT_KEY_DOWN)
        {
            // clang-format off
            if (sdl_event.key.keysym.sym == SDLK_w) e.keyboard.w_clicked         = 1;
            if (sdl_event.key.keysym.sym == SDLK_s) e.keyboard.s_clicked         = 1;
            if (sdl_event.key.keysym.sym == SDLK_a) e.keyboard.a_clicked         = 1;
            if (sdl_event.key.keysym.sym == SDLK_d) e.keyboard.d_clicked         = 1;
            if (sdl_event.key.keysym.sym == SDLK_SPACE) e.keyboard.space_clicked = 1;
            // clang-format on
            is_event = true;
        }
        else if (sdl_event.type == SDL_EVENT_KEY_UP)
        {
            // clang-format off
            if (sdl_event.key.keysym.sym == SDLK_w) e.keyboard.w_released         = 1;
            if (sdl_event.key.keysym.sym == SDLK_s) e.keyboard.s_released         = 1;
            if (sdl_event.key.keysym.sym == SDLK_a) e.keyboard.a_released         = 1;
            if (sdl_event.key.keysym.sym == SDLK_d) e.keyboard.d_released         = 1;
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
        else if (sdl_event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        {
            // clang-format off
            if (sdl_event.button.button == SDL_BUTTON_LEFT) e.mouse.left_cliked = 1;
            // clang-format on
            is_event = true;
        }
        else if (sdl_event.type == SDL_EVENT_MOUSE_BUTTON_UP)
        {
            // clang-format off
            if (sdl_event.button.button == SDL_BUTTON_LEFT) e.mouse.left_released = 1;
            // clang-format on
            is_event = true;
        }
    }
    return is_event;
}

void engine_opengl::render_triangle(const triangle<vertex3d>& tr)
{
    reload_uniform();

    glBufferData(GL_ARRAY_BUFFER, sizeof(tr), &tr, GL_STATIC_DRAW);
    GL_CHECK_ERRORS()

    bind_vertexes<vertex3d>();
    bind_normal<vertex3d>();

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void engine_opengl::render_triangle(const triangle<vertex3d_colored>& tr)
{
    reload_uniform();

    glBufferData(GL_ARRAY_BUFFER, sizeof(tr), &tr, GL_STATIC_DRAW);
    GL_CHECK_ERRORS()

    bind_vertexes<vertex3d_colored>();
    bind_normal<vertex3d_colored>();
    bind_colors<vertex3d_colored>();

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void engine_opengl::render_triangle(const triangle<vertex3d_textured>& tr)
{
    reload_uniform();

    glBufferData(GL_ARRAY_BUFFER, sizeof(tr), &tr, GL_STATIC_DRAW);
    GL_CHECK_ERRORS()

    bind_vertexes<vertex3d_textured>();
    bind_normal<vertex3d_textured>();
    bind_texture_coords<vertex3d_textured>();

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void engine_opengl::render_triangle(
    const triangle<vertex3d_colored_textured>& tr)
{
    reload_uniform();

    glBufferData(GL_ARRAY_BUFFER, sizeof(tr), &tr, GL_STATIC_DRAW);
    GL_CHECK_ERRORS()

    bind_vertexes<vertex3d_colored_textured>();
    bind_normal<vertex3d_colored_textured>();
    bind_texture_coords<vertex3d_colored_textured>();
    bind_colors<vertex3d_colored_textured>();

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void engine_opengl::render_triangles(vertex_buffer<vertex3d>* vertexes,
                                     index_buffer*            indexes,
                                     const std::uint16_t* start_vertex_index,
                                     size_t               num_vertexes)
{
    reload_uniform();

    vertexes->bind();
    indexes->bind();

    bind_vertexes<vertex3d>();
    bind_normal<vertex3d>();

    glDrawElements(
        GL_TRIANGLES, num_vertexes, GL_UNSIGNED_SHORT, start_vertex_index);

    GL_CHECK_ERRORS()
}
void engine_opengl::render_triangles(vertex_buffer<vertex3d_colored>* vertexes,
                                     index_buffer*                    indexes,
                                     const std::uint16_t* start_vertex_index,
                                     size_t               num_vertexes)
{
    reload_uniform();

    vertexes->bind();
    indexes->bind();

    bind_vertexes<vertex3d_colored>();
    bind_normal<vertex3d_colored>();
    bind_colors<vertex3d_colored>();

    glDrawElements(
        GL_TRIANGLES, num_vertexes, GL_UNSIGNED_SHORT, start_vertex_index);

    GL_CHECK_ERRORS()
}
void engine_opengl::render_triangles(vertex_buffer<vertex3d_textured>* vertexes,
                                     index_buffer*                     indexes,
                                     const texture_opengl*             tex,
                                     const std::uint16_t* start_vertex_index,
                                     size_t               num_vertexes)
{
    reload_uniform();

    vertexes->bind();
    indexes->bind();
    tex->bind();

    bind_vertexes<vertex3d_textured>();
    bind_normal<vertex3d_textured>();
    bind_texture_coords<vertex3d_textured>();

    glDrawElements(
        GL_TRIANGLES, num_vertexes, GL_UNSIGNED_SHORT, start_vertex_index);

    GL_CHECK_ERRORS()
}

void engine_opengl::render_triangles(
    vertex_buffer<vertex3d_colored_textured>* vertexes,
    index_buffer*                             indexes,
    const texture_opengl*                     tex,
    const std::uint16_t*                      start_vertex_index,
    size_t                                    num_vertexes)
{
    vertexes->bind();
    indexes->bind();
    tex->bind();

    bind_vertexes<vertex3d_colored_textured>();
    bind_normal<vertex3d_colored_textured>();
    bind_texture_coords<vertex3d_colored_textured>();
    bind_colors<vertex3d_colored_textured>();

    glDrawElements(
        GL_TRIANGLES, num_vertexes, GL_UNSIGNED_SHORT, start_vertex_index);

    GL_CHECK_ERRORS()
}

void engine_opengl::render_triangles(
    vertex_buffer<vertex2d_colored_textured>* vertexes,
    index_buffer*                             indexes,
    const texture_opengl*                     tex,
    const std::uint16_t*                      start_vertex_index,
    size_t                                    num_vertexes)
{
    vertexes->bind();
    indexes->bind();
    tex->bind();

    bind_vertexes<vertex2d_colored_textured>();
    bind_texture_coords<vertex2d_colored_textured>();
    bind_colors<vertex2d_colored_textured>();

    glDrawElements(
        GL_TRIANGLES, num_vertexes, GL_UNSIGNED_SHORT, start_vertex_index);
    GL_CHECK_ERRORS()
}

void engine_opengl::swap_buffers()
{

    ImGui_ImplSdlGL3_RenderDrawLists(this, ImGui::GetDrawData());

    SDL_GL_SwapWindow(window);

    ImGui_ImplSdlGL3_NewFrame(window);

    glClearColor(1.f, 1.f, 1.f, 1.f);
    GL_CHECK_ERRORS()

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GL_CHECK_ERRORS()
}

texture_opengl* engine_opengl::load_texture(uint32_t index, const char* path)
{
    // glActiveTexture(GL_TEXTURE0 + index);
    // GL_CHECK_ERRORS()
    // active_shader->set_uniform1("u_texture", index);

    texture_opengl* tex = new texture_opengl(path);
    tex->bind();

    return tex;
}

void engine_opengl::set_texture(uint32_t index)
{
    glActiveTexture(GL_TEXTURE0 + index);
    active_shader->set_uniform1("u_texture", index);
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
    active_shader->set_uniform1("u_uniforms.width", uniforms_world->width);
    active_shader->set_uniform1("u_uniforms.height", uniforms_world->height);

    active_shader->set_uniform1("u_uniforms.rotate_alpha_obj",
                                *uniforms_world->rotate_alpha_obj);
    active_shader->set_uniform1("u_uniforms.rotate_beta_obj",
                                *uniforms_world->rotate_beta_obj);
    active_shader->set_uniform1("u_uniforms.rotate_gamma_obj",
                                *uniforms_world->rotate_gamma_obj);

    active_shader->set_uniform1("u_uniforms.rotate_alpha_camera",
                                *uniforms_world->rotate_alpha_camera);
    active_shader->set_uniform1("u_uniforms.rotate_beta_camera",
                                *uniforms_world->rotate_beta_camera);
    active_shader->set_uniform1("u_uniforms.rotate_gamma_camera",
                                *uniforms_world->rotate_gamma_camera);

    active_shader->set_uniform1("u_uniforms.translate_x_obj",
                                *uniforms_world->translate_x_obj);
    active_shader->set_uniform1("u_uniforms.translate_y_obj",
                                *uniforms_world->translate_y_obj);
    active_shader->set_uniform1("u_uniforms.translate_z_obj",
                                *uniforms_world->translate_z_obj);

    active_shader->set_uniform1("u_uniforms.translate_x_camera",
                                *uniforms_world->translate_x_camera);
    active_shader->set_uniform1("u_uniforms.translate_y_camera",
                                *uniforms_world->translate_y_camera);
    active_shader->set_uniform1("u_uniforms.translate_z_camera",
                                *uniforms_world->translate_z_camera);

    active_shader->set_uniform1("u_uniforms.scale_x_obj",
                                *uniforms_world->scale_x_obj);
    active_shader->set_uniform1("u_uniforms.scale_y_obj",
                                *uniforms_world->scale_y_obj);
    active_shader->set_uniform1("u_uniforms.scale_z_obj",
                                *uniforms_world->scale_z_obj);
}

void engine_opengl::set_shader(shader_opengl* shader)
{
    this->active_shader = shader;

    active_shader->use();
}

// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if
// dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your
// main application.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to
// your main application.
// Generally you may always pass all inputs to dear imgui, and hide them from
// your application based on those two flags.
bool ImGui_ImplSdlGL3_ProcessEvent(const SDL_Event* event)
{
    ImGuiIO& io = ImGui::GetIO();
    switch (event->type)
    {
        case SDL_EVENT_MOUSE_WHEEL:
        {
            if (event->wheel.y > 0)
                g_MouseWheel = 1;
            if (event->wheel.y < 0)
                g_MouseWheel = -1;
            return true;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
        {
            if (event->button.button == SDL_BUTTON_LEFT)
                g_MousePressed[0] = true;
            if (event->button.button == SDL_BUTTON_RIGHT)
                g_MousePressed[1] = true;
            if (event->button.button == SDL_BUTTON_MIDDLE)
                g_MousePressed[2] = true;
            return true;
        }
        case SDL_EVENT_TEXT_INPUT:
        {
            io.AddInputCharactersUTF8(event->text.text);
            return true;
        }
        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP:
        {
            int key = event->key.keysym.scancode; // & ~SDLK_SCANCODE_MASK;
            io.KeysDown[key] = (event->type == SDL_EVENT_KEY_DOWN);
            io.KeyShift      = ((SDL_GetModState() & SDL_KMOD_SHIFT) != 0);
            io.KeyCtrl       = ((SDL_GetModState() & SDL_KMOD_CTRL) != 0);
            io.KeyAlt        = ((SDL_GetModState() & SDL_KMOD_ALT) != 0);
            io.KeySuper      = ((SDL_GetModState() & SDL_KMOD_GUI) != 0);
            return true;
        }
    }
    return false;
}

void ImGui_ImplSdlGL3_CreateFontsTexture()
{
    // Build texture atlas
    ImGuiIO&       io     = ImGui::GetIO();
    unsigned char* pixels = nullptr;
    int            width  = 0;
    int            height = 0;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    // Store our identifier
    io.Fonts->TexID = new texture_opengl(static_cast<void*>(pixels),
                                         static_cast<size_t>(width),
                                         static_cast<size_t>(height));
}

bool ImGui_ImplSdlGL3_CreateDeviceObjects()
{
    g_imgui_shader = new shader_opengl("./99-game/shaders/imgui_shader.vert",
                                       "./99-game/shaders/imgui_shader.frag");

    ImGui_ImplSdlGL3_CreateFontsTexture();

    return true;
}

void ImGui_ImplSdlGL3_InvalidateDeviceObjects()
{
    void*           ptr     = ImGui::GetIO().Fonts->TexID;
    texture_opengl* texture = reinterpret_cast<texture_opengl*>(ptr);

    delete g_imgui_shader;
    g_imgui_shader = nullptr;
}

bool ImGui_ImplSdlGL3_Init(SDL_Window* window)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard
    // Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();

    ImGuiIO& io = ImGui::GetIO();
    // g_Window    = window;

    // Setup back-end capabilities flags
    //    io.BackendFlags |=
    //        ImGuiBackendFlags_HasMouseCursors; // We can honor
    //        GetMouseCursor()
    //                                           // values (optional)
    //    io.BackendFlags |=
    //        ImGuiBackendFlags_HasSetMousePos; // We can honor
    //        io.WantSetMousePos
    //                                          // requests (optional, rarely
    //                                          used)
    io.BackendPlatformName = "imgui_impl_sdl";

    // Keyboard mapping. ImGui will use those indices to peek into the
    // io.KeysDown[] array.
    io.KeyMap[ImGuiKey_Tab]        = SDL_SCANCODE_TAB;
    io.KeyMap[ImGuiKey_LeftArrow]  = SDL_SCANCODE_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow]    = SDL_SCANCODE_UP;
    io.KeyMap[ImGuiKey_DownArrow]  = SDL_SCANCODE_DOWN;
    io.KeyMap[ImGuiKey_PageUp]     = SDL_SCANCODE_PAGEUP;
    io.KeyMap[ImGuiKey_PageDown]   = SDL_SCANCODE_PAGEDOWN;
    io.KeyMap[ImGuiKey_Home]       = SDL_SCANCODE_HOME;
    io.KeyMap[ImGuiKey_End]        = SDL_SCANCODE_END;
    io.KeyMap[ImGuiKey_Insert]     = SDL_SCANCODE_INSERT;
    io.KeyMap[ImGuiKey_Delete]     = SDL_SCANCODE_DELETE;
    io.KeyMap[ImGuiKey_Backspace]  = SDL_SCANCODE_BACKSPACE;
    io.KeyMap[ImGuiKey_Space]      = SDL_SCANCODE_SPACE;
    io.KeyMap[ImGuiKey_Enter]      = SDL_SCANCODE_RETURN;
    io.KeyMap[ImGuiKey_Escape]     = SDL_SCANCODE_ESCAPE;
    io.KeyMap[ImGuiKey_A]          = SDL_SCANCODE_A;
    io.KeyMap[ImGuiKey_C]          = SDL_SCANCODE_C;
    io.KeyMap[ImGuiKey_V]          = SDL_SCANCODE_V;
    io.KeyMap[ImGuiKey_X]          = SDL_SCANCODE_X;
    io.KeyMap[ImGuiKey_Y]          = SDL_SCANCODE_Y;
    io.KeyMap[ImGuiKey_Z]          = SDL_SCANCODE_Z;
    /*
        g_MouseCursors[ImGuiMouseCursor_Arrow] =
            SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
        g_MouseCursors[ImGuiMouseCursor_TextInput] =
            SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
        g_MouseCursors[ImGuiMouseCursor_ResizeAll] =
            SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
        g_MouseCursors[ImGuiMouseCursor_ResizeNS] =
            SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
        g_MouseCursors[ImGuiMouseCursor_ResizeEW] =
            SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
        g_MouseCursors[ImGuiMouseCursor_ResizeNESW] =
            SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
        g_MouseCursors[ImGuiMouseCursor_ResizeNWSE] =
            SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
        g_MouseCursors[ImGuiMouseCursor_Hand] =
            SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    */
    io.SetClipboardTextFn = ImGui_ImplSdlGL3_SetClipboardText;
    io.GetClipboardTextFn = ImGui_ImplSdlGL3_GetClipboardText;
    io.ClipboardUserData  = nullptr;

#ifdef _WIN32
    //SDL_SysWMinfo wmInfo;
    //SDL_VERSION(&wmInfo.version);
    //SDL_GetWindowWMInfo(window, &wmInfo);
    //io.ImeWindowHandle = wmInfo.info.win.window;
#else
    (void)window;
#endif

    g_Time = SDL_GetTicks() / 1000.f;

    return true;
}

void ImGui_ImplSdlGL3_Shutdown()
{
    ImGui_ImplSdlGL3_InvalidateDeviceObjects();
    ImGui::DestroyContext();
}

void ImGui_ImplSdlGL3_NewFrame(SDL_Window* window)
{
    ImGuiIO& io = ImGui::GetIO();

    if (io.Fonts->TexID == nullptr)
    {
        ImGui_ImplSdlGL3_CreateDeviceObjects();
    }

    // Setup display size (every frame to accommodate for window resizing)
    int w, h;
    int display_w, display_h;
    SDL_GetWindowSize(window, &w, &h);
    SDL_GetWindowSizeInPixels(window, &display_w, &display_h);
    io.DisplaySize             = ImVec2(float(w), float(h));
    io.DisplayFramebufferScale = ImVec2(w > 0 ? float(display_w / w) : 0.f,
                                        h > 0 ? float(display_h / h) : 0.f);

    // Setup time step
    Uint32 time         = SDL_GetTicks();
    float  current_time = time / 1000.0f;
    io.DeltaTime        = current_time - g_Time; // (1.0f / 60.0f);
    if (io.DeltaTime <= 0)
    {
        io.DeltaTime = 0.00001f;
    }
    g_Time = current_time;

    ImVec2   mouse_pos(-FLT_MAX, -FLT_MAX);
    uint32_t mouse_state = SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
    if (SDL_GetWindowFlags(window) & SDL_WINDOW_MOUSE_FOCUS)
    {
        io.MousePos = mouse_pos;
    }
    else
    {
        io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
    }

    io.MouseDown[0] = g_MousePressed[0] || (mouse_state & SDL_BUTTON_LEFT);
    io.MouseDown[1] = g_MousePressed[1];
    io.MouseDown[2] = g_MousePressed[2];

    std::fill_n(&g_MousePressed[0], 3, false);

    io.MouseWheel = g_MouseWheel;
    g_MouseWheel  = 0.0f;

    // Hide OS mouse cursor if ImGui is drawing it
    if (io.MouseDrawCursor)
    {
        SDL_HideCursor();
    }
    else
    {
        SDL_ShowCursor();
    }
}
