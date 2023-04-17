cmake_minimum_required(VERSION 3.26)

project(02-1-sdl-version)


find_package(SDL2 COMPONENTS REQUIRED)

# add_executable(02-1-sdl-version-dynamic main.cpp)
# target_link_libraries(02-1-sdl-version-dynamic PRIVATE SDL2::SDL2-shared)


add_executable(02-1-sdl-version-static main.cpp)
target_link_libraries(02-1-sdl-version-static PRIVATE SDL2::SDL2-static)