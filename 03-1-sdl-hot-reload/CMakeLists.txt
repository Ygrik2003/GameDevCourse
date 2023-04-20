cmake_minimum_required(VERSION 3.26)

project(03-1-sdl-hot-reload)

add_subdirectory(dll_injection)
add_subdirectory(engine)

add_library(03-1-game-lib SHARED game.cpp)

target_compile_features(03-1-game-lib PUBLIC cxx_std_20)
target_link_libraries(03-1-engine PRIVATE 03-1-dll-injection)