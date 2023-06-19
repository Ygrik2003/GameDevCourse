cmake_minimum_required(VERSION 3.20)

project(04-3-render-morphing)


add_executable(04-3-render-morphing main.cpp
               glad/glad.h glad/glad.c
               core/config.h core/event.h
               engine/engine.h engine/engine.cpp
               KHR/khrplatform.h
               morphing_engine.h morphing_engine.cpp
               game.h game.cpp
               objects/object.h objects/object.cpp
               objects/sphere_culinder.h objects/sphere_culinder.cpp
               objects/sphere.h objects/sphere.cpp
               objects/culinder.h objects/culinder.cpp
               objects/camera.h objects/camera.cpp 
               )
target_include_directories(04-3-render-morphing PRIVATE
                           ${CMAKE_CURRENT_SOURCE_DIR})
set_property(TARGET 04-3-render-morphing PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})


find_package(SDL3 REQUIRED)
find_package(OpenGL REQUIRED)


target_compile_features(04-3-render-morphing PUBLIC cxx_std_20)
target_link_libraries(04-3-render-morphing PUBLIC SDL3::SDL3 OpenGL::GL)