#include <chrono>
#include <iostream>
#include <thread>

#include "engine/engine.h"

class my_game : public yg::game
{
public:
    explicit my_game(yg::engine& e, config conf) { this->conf = conf; }
    void initialize() override
    {
        std::cout << conf.char_start << std::flush;
        for (int i = 0; i < conf.max_chars; i++)
            std::cout << " " << std::flush;
        std::cout << conf.char_end << std::flush;

        for (int i = 0; i < conf.max_chars + 1; i++)
            std::cout << "\b" << std::flush;
    }
    void on_event(yg::event) override {}
    void update() override
    {
        if (currentStep < conf.max_chars)
        {
            std ::cout << conf.char_placeholder << std::flush;
            currentStep++;
        }
        else
        {
            for (int i = 0; i < currentStep; i++)
                std::cout << "\b \b" << std::flush;
            currentStep = 0;
        }
    }
    void render() const override {}

private:
    size_t currentStep = 0;
    config conf;
};

yg::game* start_game(yg::engine* engine)
{
    if (engine != nullptr)
    {
        return new my_game(*engine, config(5, '-', '[', ']', 2));
    }
    return nullptr;
}

void destroy_game(yg::game* game)
{
    if (game)
        delete game;
}