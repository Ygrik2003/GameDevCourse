cmake_minimum_required(VERSION 3.20)

project(99-game CXX)

find_package(SDL3 REQUIRED)
find_package(OpenGL REQUIRED)

add_library(99-engine STATIC
            glad/glad.h glad/glad.c
            KHR/khrplatform.h
            engine/engine.h engine/engine.cpp
            core/config.h core/event.h
            core/types.h core/types.cpp objects/figure.h 
            objects/camera.h objects/camera.cpp
            objects/mesh.h objects/mesh.cpp
            objects/model.h objects/model.cpp
            objects/object.h objects/object.cpp
            core/picopng.hxx)
target_include_directories(99-engine PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_compile_features(99-engine PUBLIC cxx_std_20)
target_link_libraries(99-engine SDL3::SDL3-shared OpenGL::GL -lassimp)





add_executable(99-game main.cpp game.h game.cpp)
target_include_directories(99-game PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_compile_features(99-game PUBLIC cxx_std_20)
target_link_libraries(99-game PRIVATE 99-engine)