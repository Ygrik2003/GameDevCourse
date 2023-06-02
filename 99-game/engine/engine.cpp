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
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(vertex_type),
                          reinterpret_cast<GLvoid*>(0));
    GL_CHECK_ERRORS()
}

template <class vertex_type>
void bind_normals()
{
    glEnableVertexAttribArray(1);
    GL_CHECK_ERRORS()
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(vertex_type),
                          reinterpret_cast<GLvoid*>(3 * sizeof(float)));
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
        reinterpret_cast<GLvoid*>(sizeof(vertex_type) - 2 * sizeof(float)));
    GL_CHECK_ERRORS()
}

template <class vertex_colored_type>
void bind_colors()
{
    glEnableVertexAttribArray(3);
    GL_CHECK_ERRORS()
    glVertexAttribPointer(3,
                          4,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(vertex_colored_type),
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

static float          g_Time            = 0.0;
static bool           g_MousePressed[3] = { false, false, false };
static float          g_MouseWheel      = 0.0f;
static shader_opengl* g_im_gui_shader   = nullptr;

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
    e.clear();
    bool      is_event = false;
    SDL_Event sdl_event;

    if (SDL_PollEvent(&sdl_event))
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

    bind_vertexes<vertex>();
    bind_normals<vertex>();

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void engine_opengl::render_triangle(const triangle<vertex_colored>& tr)
{
    reload_uniform();

    glBufferData(GL_ARRAY_BUFFER, sizeof(tr), &tr, GL_STATIC_DRAW);
    GL_CHECK_ERRORS()

    bind_vertexes<vertex_colored>();
    bind_normals<vertex_colored>();
    bind_colors<vertex_colored>();

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void engine_opengl::render_triangle(const triangle<vertex_textured>& tr)
{
    reload_uniform();

    glBufferData(GL_ARRAY_BUFFER, sizeof(tr), &tr, GL_STATIC_DRAW);
    GL_CHECK_ERRORS()

    bind_vertexes<vertex_textured>();
    bind_normals<vertex_textured>();
    bind_texture_coords<vertex_textured>();

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void engine_opengl::render_triangle(const triangle<vertex_colored_textured>& tr)
{
    reload_uniform();

    glBufferData(GL_ARRAY_BUFFER, sizeof(tr), &tr, GL_STATIC_DRAW);
    GL_CHECK_ERRORS()

    bind_vertexes<vertex_colored_textured>();
    bind_normals<vertex_colored_textured>();
    bind_texture_coords<vertex_colored_textured>();
    bind_colors<vertex_colored_textured>();

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void engine_opengl::render_triangles(vertex_buffer<vertex>* vertexes,
                                     index_buffer*          indexes,
                                     const std::uint32_t*   start_vertex_index,
                                     size_t                 num_vertexes)
{
    reload_uniform();

    vertexes->bind();
    indexes->bind();

    bind_vertexes<vertex>();
    bind_normals<vertex>();

    glDrawElements(
        GL_TRIANGLES, num_vertexes, GL_UNSIGNED_SHORT, start_vertex_index);

    GL_CHECK_ERRORS()
}
void engine_opengl::render_triangles(vertex_buffer<vertex_colored>* vertexes,
                                     index_buffer*                  indexes,
                                     const std::uint32_t* start_vertex_index,
                                     size_t               num_vertexes)
{
    reload_uniform();

    vertexes->bind();
    indexes->bind();

    bind_vertexes<vertex_colored>();
    bind_normals<vertex_colored>();
    bind_colors<vertex_colored>();

    glDrawElements(
        GL_TRIANGLES, num_vertexes, GL_UNSIGNED_SHORT, start_vertex_index);

    GL_CHECK_ERRORS()
}
void engine_opengl::render_triangles(vertex_buffer<vertex_textured>* vertexes,
                                     index_buffer*                   indexes,
                                     const texture_opengl*           tex,
                                     const std::uint32_t* start_vertex_index,
                                     size_t               num_vertexes)
{
    reload_uniform();

    vertexes->bind();
    indexes->bind();
    tex->bind();

    bind_vertexes<vertex_textured>();
    bind_normals<vertex_textured>();
    bind_texture_coords<vertex_textured>();

    glDrawElements(
        GL_TRIANGLES, num_vertexes, GL_UNSIGNED_SHORT, start_vertex_index);

    GL_CHECK_ERRORS()
}

void engine_opengl::render_triangles(
    vertex_buffer<vertex_colored_textured>* vertexes,
    index_buffer*                           indexes,
    const texture_opengl*                   tex,
    const std::uint32_t*                    start_vertex_index,
    size_t                                  num_vertexes)
{
    reload_uniform();

    vertexes->bind();
    indexes->bind();
    tex->bind();

    bind_vertexes<vertex_colored_textured>();
    bind_normals<vertex_colored_textured>();
    bind_texture_coords<vertex_colored_textured>();
    bind_colors<vertex_colored_textured>();

    glDrawElements(
        GL_TRIANGLES, num_vertexes, GL_UNSIGNED_SHORT, start_vertex_index);

    GL_CHECK_ERRORS()
}

void engine_opengl::swap_buffers()
{
    SDL_GL_SwapWindow(window);

    glClearColor(1.f, 1.f, 1.f, 1.f);
    GL_CHECK_ERRORS()

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GL_CHECK_ERRORS()
}

texture_opengl* engine_opengl::load_texture(size_t index, const char* path)
{
    glActiveTexture(GL_TEXTURE0 + index);
    GL_CHECK_ERRORS()
    glUniform1i(uniform_texture, index);

    texture_opengl* tex = new texture_opengl(path);
    tex->bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    GL_CHECK_ERRORS()
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    GL_CHECK_ERRORS()
    return tex;
}

void engine_opengl::set_texture(size_t index)
{
    glActiveTexture(GL_TEXTURE0 + index);
    glUniform1i(uniform_texture, index);
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

void imgui_render()
{
    ImDrawData* draw_data = ImGui::GetDrawData();
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
    assert(texture != nullptr);

    // om::mat2x3 orto_matrix =
    //     om::mat2x3::scale(2.0f / io.DisplaySize.x, -2.0f / io.DisplaySize.y)
    //     * om::mat2x3::move(om::vec2(-1.0f, 1.0f));

    g_im_gui_shader->use();
    // g_im_gui_shader->set_uniform("Texture", texture);
    // g_im_gui_shader->set_uniform("ProjMtx", orto_matrix);

    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list          = draw_data->CmdLists[n];
        const ImDrawIdx*  idx_buffer_offset = nullptr;

        const vertex_colored_textured* vertexes =
            reinterpret_cast<vertex_colored_textured*>(
                cmd_list->VtxBuffer.Data);
        size_t vert_count = static_cast<size_t>(cmd_list->VtxBuffer.size());

        vertex_buffer<vertex_colored_textured>* vertex_buff =
            new vertex_buffer(vertexes, vert_count);

        const std::uint16_t* indexes = cmd_list->IdxBuffer.Data;
        size_t index_count = static_cast<size_t>(cmd_list->IdxBuffer.size());

        index_buffer* index_buff = new index_buffer(indexes, index_count);

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            assert(pcmd->UserCallback == nullptr); // we not use it

            texture_opengl* tex =
                reinterpret_cast<texture_opengl*>(pcmd->TextureId);

            // render(vertexes, indexes, tex, idx_buffer_offset,
            // pcmd->ElemCount);

            idx_buffer_offset += pcmd->ElemCount;
        } // end for cmd_i
        // om::g_engine->destroy_vertex_buffer(vertex_buff);
        // om::g_engine->destroy_index_buffer(index_buff);
    } // end for n
}

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
            int key          = event->key.keysym.sym & ~SDLK_SCANCODE_MASK;
            io.KeysDown[key] = (event->type == SDL_EVENT_KEY_DOWN);
            uint32_t mod_keys_mask = SDL_GetModState();
            io.KeyShift            = ((mod_keys_mask & SDL_KMOD_SHIFT) != 0);
            io.KeyCtrl             = ((mod_keys_mask & SDL_KMOD_CTRL) != 0);
            io.KeyAlt              = ((mod_keys_mask & SDL_KMOD_ALT) != 0);
            io.KeySuper            = ((mod_keys_mask & SDL_KMOD_GUI) != 0);
            return true;
        }
    }
    return false;
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
    // io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors; // We can honor
    // GetMouseCursor()
    //                                           // values (optional)
    //    io.BackendFlags |=
    //        ImGuiBackendFlags_HasSetMousePos; // We can honor
    //        io.WantSetMousePos
    //                                          // requests (optional, rarely
    //                                          used)
    io.BackendPlatformName = "custom_micro_engine";

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
    // io.RenderDrawListsFn =
    //     imgui_to_engine_render; // Alternatively you can set this to
    //                             // NULL and call ImGui::GetDrawData()
    //                             // after ImGui::Render() to get the
    //                             // same ImDrawData pointer.
    // io.SetClipboardTextFn = ImGui_ImplSdlGL3_SetClipboardText;
    // io.GetClipboardTextFn = ImGui_ImplSdlGL3_GetClipboardText;
    // io.ClipboardUserData  = nullptr;

#ifdef _WIN32
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(window, &wmInfo);
    io.ImeWindowHandle = wmInfo.info.win.window;
#else
    (void)window;
#endif

    g_Time = SDL_GetTicks() / 1000.f;

    return true;
}
