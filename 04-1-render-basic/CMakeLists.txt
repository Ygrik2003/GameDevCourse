cmake_minimum_required(VERSION 3.26)

project(04-1-render-basic)

add_subdirectory(canvas)
add_executable(04-1-render-basic canvas_basic_main.cpp)

target_include_directories(04-1-render-basic PUBLIC
                           ${CMAKE_CURRENT_SOURCE_DIR}/canvas
)
target_link_libraries(04-1-render-basic PRIVATE 04-1-canvas)