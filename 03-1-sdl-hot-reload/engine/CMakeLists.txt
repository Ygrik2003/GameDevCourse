cmake_minimum_required(VERSION 3.26)

project(03-1-sdl-hot-reload)

add_executable(03-1-engine engine.h engine.cpp core.h)
target_compile_features(03-1-engine PUBLIC cxx_std_20)

find_package(SDL3 REQUIRED)

target_link_libraries(03-1-engine PRIVATE SDL3::SDL3 03-1-dll-injection 03-1-game-lib)