#include "dll_injection.h"

dll_injection::dll_injection(yg::engine* engine,
                             const char* name_dll,
                             const char* name_dll_temp,
                             const char* name_start_function,
                             const char* name_stop_function)
{
    this->engine              = engine;
    this->name_dll            = name_dll;
    this->name_dll_temp       = name_dll_temp;
    this->name_start_function = name_start_function;
    this->name_stop_function  = name_stop_function;

    if (boost::filesystem::exists(this->name_dll))
    {

        if (boost::filesystem::exists(this->name_dll_temp))
            boost::filesystem::remove(name_dll_temp);
        boost::filesystem::copy(name_dll, name_dll_temp);

        dll_pointers.push_back(SDL_LoadObject(name_dll_temp));
        version = 0;

        if (dll_pointers[version] == nullptr)
        {
            std::cerr << "error: failed to load: [" << this->name_dll << "] "
                      << SDL_GetError() << std::endl;
        }
        start();
    }
    else
    {
        std::cerr << "error: No file .so: " << std::endl;
    }

    // DLL window init and create base interface for control dll injection

    const int init_result = SDL_Init(SDL_INIT_EVERYTHING);
    if (init_result != 0)
    {
        const char* err_message = SDL_GetError();
        std::cerr << "error: failed call SDL_Init: " << err_message
                  << std::endl;
        return;
    }

    SDL_Window* const window = SDL_CreateWindow(
        "DLL Injection Control Panel", 200, 480, ::SDL_WINDOW_OPENGL);

    if (window == nullptr)
    {
        const char* err_message = SDL_GetError();
        std::cerr << "error: failed call SDL_CreateWindow: " << err_message
                  << std::endl;
        SDL_Quit();
        return;
    }
    if (window != nullptr)
    {
        SDL_Renderer* renderer =
            SDL_CreateRenderer(window, "opengl", SDL_RENDERER_ACCELERATED);
        if (renderer == nullptr)
        {
            std::cerr << SDL_GetError() << std::endl;
            SDL_Quit();
            return;
        }
        SDL_RenderPresent(renderer);
    }
}

yg::game* dll_injection::update_dll()
{
    if (game != nullptr)
    {
        stop();
        SDL_UnloadObject(dll_pointers[version]);
    }
    if (boost::filesystem::exists(name_dll))
    {
        if (boost::filesystem::exists(this->name_dll_temp))
            boost::filesystem::remove(name_dll_temp);
        boost::filesystem::copy(name_dll, name_dll_temp);

        dll_pointers[version] = SDL_LoadObject(name_dll_temp);
        version               = 0;

        if (dll_pointers[version] == nullptr)
        {
            std::cerr << "error: failed to load: [" << this->name_dll << "] "
                      << SDL_GetError() << std::endl;
        }
        restart();
    }
    else
    {
        std::cerr << "error: No file .so: " << std::endl;
    }

    return game;
}

void dll_injection::add_dll()
{
    dll_pointers.push_back(SDL_LoadObject(name_dll));
}

void dll_injection::change_dll(size_t index)
{
    if (index < dll_pointers.size())
        version = index;
    else
        std::cerr << "Dll version with number " << index << "doesnt exist.";
    this->restart();
}

yg::game* dll_injection::start()
{
    SDL_FunctionPointer start_game_func_ptr =
        SDL_LoadFunction(dll_pointers[version], "start_game");

    if (start_game_func_ptr == nullptr)
    {
        std::cerr << "error: no function [start_game] "
                  << " " << SDL_GetError() << std::endl;
        return nullptr;
    }
    using start_game_ptr = decltype(&start_game);
    auto start_game_func =
        reinterpret_cast<start_game_ptr>(start_game_func_ptr);

    game = start_game_func(engine);
    return game;
}
void dll_injection::stop()
{
    SDL_FunctionPointer destroy_game_func_ptr =
        SDL_LoadFunction(dll_pointers[version], "destroy_game");

    if (destroy_game_func_ptr == nullptr)
    {
        std::cerr << "error: no function [destroy_game] "
                  << " " << SDL_GetError() << std::endl;
        return;
    }
    using destroy_game_ptr = decltype(&destroy_game);

    auto destroy_game_func =
        reinterpret_cast<destroy_game_ptr>(destroy_game_func_ptr);

    delete game;
    // destroy_game_func(game);
}
yg::game* dll_injection::restart()
{
    std::cout << '\n' << "Restarting..." << '\n';

    game = start();
    game->initialize();
    return game;
}

yg::game* dll_injection::get_game()
{
    return game;
}