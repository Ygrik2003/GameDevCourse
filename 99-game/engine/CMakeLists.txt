cmake_minimum_required(VERSION 3.20)

project(99-engine CXX)


add_library(99-engine SHARED engine.h engine.cpp)
target_compile_features(99-engine PUBLIC cxx_std_20)
target_include_directories(99-engine PUBLIC ../)

