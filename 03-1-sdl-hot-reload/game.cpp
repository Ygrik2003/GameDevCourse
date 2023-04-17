#include <iostream>

#include "engine/engine.h"

class my_game : public yg::game
{
public:
    explicit my_game(yg::engine&) {}
    void initialize() override {}
    void on_event(yg::event) override {}
    void update() override {}
    void render() const override {}

private:
};

yg::game* start_game(yg::engine* engine)
{
    if (engine != nullptr)
    {
        return new my_game(*engine);
    }
    return nullptr;
}

void destroy_game(yg::game* game)
{
    delete game;
}