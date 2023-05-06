#include "engine.h"

class sphere_engine : public engine
{
public:
    sphere_engine();

    int  initialize(config _config) override;
    void uninitialize() override;
    bool event_keyboard(event& e) override;

    void swap_buffers() override;

    GLuint load_shader(const char* path, int type) override;

private:
    void render_triangle(const triangle&) override;

    config        _config;
    SDL_Window*   window;
    SDL_GLContext gl_context;
    GLuint        handle_program;
};