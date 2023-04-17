#include <iostream>
#include <memory>

#include "../dll_injection/dll_injection.h"
#include "engine.h"

class engine_impl final : public yg::engine
{
public:
    int initialize(config conf) final { return 0; }

    bool read_input() final { return false; }
    void uninitialize() final {}
};

yg::engine::~engine() = default;

int main()
{
    yg::engine* engine = new engine_impl();

    engine->initialize(config(5, '#', '[', ']', 2));

    std::cout << "start app\n" << std::endl;

    dll_injection dll_inj(
        engine, "../lib03-1-game-lib.so", "", "start_game", "destroy_game");

    yg::game* game          = dll_inj.start();
    bool      continue_loop = true;
    while (continue_loop)
    {

        while (engine->read_input())
        {
        }

        game->update();
        game->render();
    }

    engine->uninitialize();

    return EXIT_SUCCESS;
}

static bool already_exist = false;

yg::engine* create_engine()
{
    if (already_exist)
    {
        throw std::runtime_error("engine already exist");
    }
    yg::engine* result = new engine_impl();
    already_exist      = true;
    return result;
}

void destroy_engine(yg::engine* e)
{
    if (already_exist)
    {
        if (nullptr == e)
        {
            throw std::runtime_error("e is nullptr");
        }
        delete e;
    }
    else
    {
        throw std::runtime_error("engine not created");
    }
}