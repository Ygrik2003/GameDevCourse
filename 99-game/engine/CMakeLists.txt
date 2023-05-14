cmake_minimum_required(VERSION 3.20)

project(99-engine CXX)


find_package(SDL3 REQUIRED)
find_package(OpenGL REQUIRED)

add_library(99-engine SHARED engine.h engine.cpp
            ../core/config.h ../core/event.h
            ../core/types.h ../core/picopng.hxx)
target_compile_features(99-engine PUBLIC cxx_std_20)
target_link_libraries(99-engine SDL3::SDL3-shared OpenGL::GL)
target_include_directories(99-engine PUBLIC ../)

