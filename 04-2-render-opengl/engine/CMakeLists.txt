cmake_minimum_required(VERSION 3.20)

project(04-2-engine CXX)


add_library(04-2-engine SHARED engine.h engine.cpp)
target_compile_features(04-2-engine PUBLIC cxx_std_20)
target_include_directories(04-2-engine PUBLIC ../)
