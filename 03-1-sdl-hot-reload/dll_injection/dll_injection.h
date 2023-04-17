#pragma once
#include <iostream>
#include <vector>

#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <boost/signals2.hpp>

#include <SDL3/SDL.h>

#include "../engine/engine.h"

class dll_injection
{
public:
    dll_injection(yg::engine* engine,
                  const char* name_dll,
                  const char* name_dll_temp,
                  const char* name_start_function,
                  const char* name_stop_function);

    boost::signals2::signal<void()> fileChanged;

    void update_dll();
    void change_dll(size_t index);

    yg::game* start();
    void      stop();
    yg::game* restart();

private:
    const char* name_dll            = nullptr;
    const char* name_dll_temp       = nullptr;
    const char* name_start_function = nullptr;
    const char* name_stop_function  = nullptr;

    std::vector<void*> dll_pointers;
    size_t             version = -1;

    bool        isStarted = false;
    yg::engine* engine;
    yg::game*   game;
};