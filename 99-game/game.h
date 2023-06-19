#pragma once
#include "core/config.h"
#include "core/event.h"
#include "core/types.h"
#include "engine/engine_opengl.h"
#include "objects/camera.h"

/*
0100 0100 0100 0110
0100 1110 0100 0110
0110 0000 0100 0000
0000 0000 0100 0000

*/

constexpr uint32_t fps = 120;

enum class direction
{
    left,
    forward,
    right,
    backward,
    down,
    up
};

struct cell
{
    bool is_free       = true;
    bool is_moving     = true;
    bool is_controlled = true;

    uint8_t x = 0;
    uint8_t y = 0;
    uint8_t z = 0;

    cell* next = nullptr;
    cell* prev = nullptr;

    //?
    void set_moving(bool state)
    {
        if (is_moving == state)
            return;
        is_moving = state;
        if (next)
            next->set_moving(state);
        if (prev)
            prev->set_moving(state);
    }
    //?
    void set_controlling(bool state)
    {
        if (is_controlled == state)
            return;
        is_controlled = state;
        if (next)
            next->set_controlling(state);
        if (prev)
            prev->set_controlling(state);
    }
};

constexpr size_t cells_max    = 8;
constexpr size_t cells_max_z  = 14;
constexpr size_t cells_z_lose = 10;

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

    void add_figure(figure* fig, texture* texture);

private:
    void draw_menu();
    void draw_ui();
    void render_scene();
    void start_game();

    void new_primitive();
    void add_primitive(size_t number);
    
    std::chrono::steady_clock timer;
    time_point                last_time_update;
    float                     delay = 0.5; // Seconds



    // Game func's
    config cfg;
    size_t score = 0;

    engine* my_engine = nullptr;
    camera* cam       = nullptr;
    uniform uniforms;
    std::vector<figure*> figures;


    std::vector<cell>   cells;
    cell*               controlled_cell = nullptr;
    std::vector<size_t> buffer_z;

    shader* shader_scene;
    shader* shader_temp;

    figure* figure_board;
    figure* figure_cube;

    texture* texture_board = nullptr;
    texture* texture_block = nullptr;

    double camera_angle = -M_PI / 2;
    double view_height  = 1.;
    double theta        = 0;

    struct flags
    {
        uint8_t is_started : 1;
        uint8_t is_quit : 1;
        uint8_t is_rotated : 1;
        uint8_t is_moving : 1;

    } state;
};