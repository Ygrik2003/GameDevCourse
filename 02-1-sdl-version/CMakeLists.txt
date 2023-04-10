cmake_minimum_required(VERSION 3.26)

project(02-1-sdl-version)

find_package(SDL3 COMPONENTS REQUIRED)

add_executable(02-1-sdl-version-dynamic main.cpp)
target_link_libraries(02-1-sdl-version-dynamic PRIVATE SDL3::SDL3-shared)


add_executable(02-1-sdl-version-static main.cpp)
target_link_libraries(02-1-sdl-version-static PRIVATE SDL3::SDL3-static)