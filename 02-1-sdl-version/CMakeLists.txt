cmake_minimum_required(VERSION 3.26)

project(02-1-sdl-version)


find_package(SDL3 COMPONENTS REQUIRED)

# add_executable(02-1-sdl-version-dynamic main.cpp)
# target_link_libraries(02-1-sdl-version-dynamic PRIVATE SDL2::SDL2-shared)


add_executable(02-1-sdl-version-static main.cpp)
message(${CMAKE_BINARY_DIR})
target_link_directories(02-1-sdl-version-static PUBLIC
						   ${CMAKE_BINARY_DIR}/lib/SDL)
target_link_libraries(02-1-sdl-version-static PRIVATE SDL3::SDL3)