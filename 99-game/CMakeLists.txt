cmake_minimum_required(VERSION 3.20)

project(99-game CXX)
# set(WORKING_DIRECTORY ${CMAKE_WORK})
# message(${WORKING_DIRECTORY})

find_package(SDL3 REQUIRED)
find_package(OpenGL REQUIRED)

add_library(99-engine STATIC
            engine/engine.h engine/engine.cpp
            core/config.h core/event.h
            core/types.h objects/figure.h 
            objects/camera.h objects/camera.cpp
            objects/object.h objects/object.cpp
            objects/checker.h objects/checker.cpp
            objects/chessboard.h objects/chessboard.cpp
            objects/chessboard_cells.h objects/chessboard_cells.cpp
            core/picopng.hxx)
target_include_directories(99-engine PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_compile_features(99-engine PUBLIC cxx_std_20)
target_link_libraries(99-engine SDL3::SDL3-shared OpenGL::GL)





add_executable(99-game main.cpp game.h game.cpp)
target_include_directories(99-game PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_compile_features(99-game PUBLIC cxx_std_20)
target_link_libraries(99-game PRIVATE 99-engine)