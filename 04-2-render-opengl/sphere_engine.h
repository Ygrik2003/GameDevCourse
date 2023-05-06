#include "engine.h"

class sphere_engine : public engine
{
public:
    sphere_engine();

    int  initialize(config _config) override;
    void uninitialize() override;
    bool event_keyboard(event& e) override;

    void update() override;
    void render(const triangles&) override;

    void   swap_buffers() override;
    GLuint load_shader(const char* path, int type) override;

    void render_triangle(const triangle&) override;

private:
    config        _config;
    SDL_Window*   window;
    SDL_GLContext gl_context;
    GLuint        handle_program;
};