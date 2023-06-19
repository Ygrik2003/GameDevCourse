#include <iostream>
#include <memory>

#include "../dll_injection/dll_injection.h"
#include "engine.h"

class engine_impl final : public yg::engine
{
public:
    int initialize(config conf) final
    {
        this->conf = conf;
        return 0;
    }

    bool read_input(yg::event& event) final
    {

        using namespace std;
        // collect all events from SDL
        SDL_Event sdl_event;
        if (SDL_PollEvent(&sdl_event))
        {
            if (sdl_event.type == SDL_EVENT_QUIT)
            {
                event.action.quit = true;
                return true;
            }
            else if (sdl_event.type == SDL_EVENT_KEY_DOWN)
            {
                if (sdl_event.key.keysym.sym == SDLK_w)
                {
                    event.keyboard.up_clicked = 1;
                    return true;
                }
            }
        }
        return false;
    }
    void uninitialize() final {}

    config get_config() final { return conf; };
};

yg::engine::~engine() = default;

int main()
{
    yg::engine* engine = new engine_impl();
    engine->initialize(config(5, '#', '[', ']', 2));

    std::cout << "start app\n" << std::endl;

    dll_injection dll_inj(engine,
                          "../lib03-1-game-lib.so",
                          "../lib03-1-game-lib-temp.so",
                          "start_game",
                          "destroy_game");

    yg::game* game = dll_inj.get_game();
    game->initialize();
    bool continue_loop = true;

    while (continue_loop)
    {

        yg::event event;
        while (engine->read_input(event))
        {
            if (event.keyboard.up_clicked)
                game = dll_inj.update_dll();
            if (event.action.quit)
            {
                continue_loop = false;
            }
        }

        game->update();
        game->render();
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
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