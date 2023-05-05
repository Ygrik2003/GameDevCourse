cmake_minimum_required(VERSION 3.20)

project(99-game CXX)

add_subdirectory(engine)

add_executable(99-game main.cpp game.h game.cpp)
target_compile_features(99-game PUBLIC cxx_std_20)
target_link_libraries(99-game 99-engine)