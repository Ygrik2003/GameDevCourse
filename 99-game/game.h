#pragma once
#include "core/config.h"
#include "core/event.h"
#include "core/types.h"
#include "engine/engine.h"
#include "objects/chessboard_cells.h"

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

class game_checkers : public game
{
public:
    int  initialize(config);
    bool event_listener(event&);
    void update();
    void render();

    void add_figure(figure&, const char* texture, size_t texture_index);

private:
    config cfg;

    engine_tetris* my_engine;
    uniform        uniforms;
    camera         cam;

    std::vector<figure*> figures;

    double phi   = 0;
    double theta = 0;
};