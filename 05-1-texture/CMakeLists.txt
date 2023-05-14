cmake_minimum_required(VERSION 3.20)

project(05-1-texture)

add_executable(05-1-texture main.cpp
               glad/glad.h glad/glad.c
               core/config.h core/event.h
               engine/engine.h engine/engine.cpp
               KHR/khrplatform.h
               engine_impl.h engine_impl.cpp
               game.h game.cpp
               objects/object.h objects/object.cpp
               objects/camera.h objects/camera.cpp 
               )
target_include_directories(05-1-texture PRIVATE
                           ${CMAKE_CURRENT_SOURCE_DIR})

find_package(SDL3 REQUIRED)
find_package(OpenGL REQUIRED)

target_compile_features(05-1-texture PUBLIC cxx_std_20)
target_link_libraries(05-1-texture PUBLIC SDL3::SDL3 OpenGL::GL)