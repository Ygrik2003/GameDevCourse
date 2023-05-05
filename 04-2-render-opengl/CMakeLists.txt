cmake_minimum_required(VERSION 3.20)

project(04-2-render-opengl)

add_subdirectory(engine)

add_executable(04-2-render-sphere main.cpp 
               engine/engine.h 
               core/config.h core/event.h
               sphere_engine.h sphere_engine.cpp
               game.h game.cpp)

target_include_directories(04-2-render-sphere PRIVATE
                           ${CMAKE_CURRENT_SOURCE_DIR}/engine
                           ${CMAKE_CURRENT_SOURCE_DIR}/core)


find_package(SDL3 REQUIRED)
find_package(OpenGL REQUIRED)

target_compile_features(04-2-render-sphere PUBLIC cxx_std_20)
target_link_libraries(04-2-render-sphere PUBLIC 04-2-engine SDL3::SDL3 OpenGL::GL)