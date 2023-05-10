cmake_minimum_required(VERSION 3.20)

project(04-2-render-opengl)

add_executable(04-2-render-sphere main.cpp
               glad/glad.h glad/glad.c
               core/config.h core/event.h
               engine/engine.h engine/engine.cpp
               KHR/khrplatform.h
               sphere_engine.h sphere_engine.cpp
               game.h game.cpp
               objects/object.h objects/object.cpp
               objects/sphere.h objects/sphere.cpp
               objects/culinder.h objects/culinder.cpp
               objects/camera.h objects/camera.cpp 
               )

target_include_directories(04-2-render-sphere PRIVATE
                           ${CMAKE_CURRENT_SOURCE_DIR})


find_package(SDL3 REQUIRED)
find_package(OpenGL REQUIRED)

target_compile_features(04-2-render-sphere PUBLIC cxx_std_20)
target_link_libraries(04-2-render-sphere PUBLIC SDL3::SDL3 OpenGL::GL)