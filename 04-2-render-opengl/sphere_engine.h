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

    void render_triangle(const triangle&        tr,
                         transformation_object& uniforms_1,
                         transformation_camera& uniforms_2) override;

private:
    config        _config;
    SDL_Window*   window;
    SDL_GLContext gl_context;
    GLuint        handle_program;

    GLuint uniform_tr_obj_rotate;
    GLuint uniform_tr_obj_scale;
    GLuint uniform_tr_obj_translate;

    GLuint uniform_tr_cam_rotate;
    GLuint uniform_tr_cam_scale;
    GLuint uniform_tr_cam_translate;
    GLuint uniform_tr_cam_projection;
};