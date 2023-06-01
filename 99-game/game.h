#pragma once
#include "core/config.h"
#include "core/event.h"
#include "core/types.h"
#include "engine/engine.h"

#include "imgui/imgui.h"

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
    int  initialize(config) override;
    bool event_listener(event&) override;
    void update() override;
    void render() override;

    void add_figure(figure, const char* texture, size_t texture_index);

private:
    void render_menu();
    void render_scene();

    config cfg;

    engine_tetris* my_engine;
    uniform        uniforms;
    camera*        cam;

    std::vector<figure> figures;

    double phi   = 0;
    double theta = 0;

    bool isStarted = false;
};