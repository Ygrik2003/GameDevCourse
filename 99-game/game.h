#pragma once
#include "core/config.h"
#include "core/event.h"
#include "core/types.h"
#include "engine/engine.h"

constexpr uint32_t fps = 60;

class game
{
public:
    virtual ~game()                     = default;
    virtual int  initialize(config)     = 0;
    virtual bool event_listener(event&) = 0;
    virtual void update()               = 0;
    virtual void render()               = 0;
};

class game_tetris : public game
{
public:
    game_tetris();

    int  initialize(config) override;
    bool event_listener(event&) override;
    void update() override;
    void render() override;

    void add_figure(figure, const char* texture, size_t texture_index);

private:
    void render_menu();
    void render_scene();
    void start_game();

    config cfg;

    engine_opengl* my_engine;
    uniform        uniforms;
    camera*        cam;

    std::vector<figure>          figures;
    std::vector<texture_opengl*> textures;
    shader_opengl*               shader_scene;
    shader_opengl*               shader_temp;

    double phi         = 0;
    double view_height = 1.5;
    double theta       = 0;

    struct flags
    {
        uint8_t is_started : 1;
        uint8_t is_quit : 1;
        uint8_t is_rotated : 1;
        uint8_t is_moving : 1;

    } state;
};