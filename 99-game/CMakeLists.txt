cmake_minimum_required(VERSION 3.20)

project(99-game CXX)

find_package(OpenGL REQUIRED)
find_package(SDL3 REQUIRED)

add_library(
    99-engine STATIC
    core/config.h
    core/event.h
    core/physics.cpp
    core/physics.h
    core/picopng.hxx
    core/types.cpp
    core/types.h
    engine/audio_buffer.cpp
    engine/audio_buffer.h
    engine/engine.h
    engine/engine_opengl.cpp
    engine/engine_opengl.h
    engine/index_buffer.cpp
    engine/index_buffer.h
    engine/shader.h
    engine/shader_opengl.cpp
    engine/shader_opengl.h
    engine/texture.h
    engine/texture_opengl.cpp
    engine/texture_opengl.h
    engine/vertex_buffer.cpp
    engine/vertex_buffer.h
    glad/glad.c
    glad/glad.h
    KHR/khrplatform.h
    objects/camera.cpp
    objects/camera.h
    objects/figure.cpp
    objects/figure.h
    objects/mesh.cpp
    objects/mesh.h
    objects/model.cpp
    objects/model.h
    objects/object.cpp
    objects/object.h
    objects/primitive.cpp
    objects/primitive.h)
target_include_directories(
    99-engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/modules
                      ${CMAKE_SOURCE_DIR}/modules/SDL3/include)
target_compile_features(99-engine PUBLIC cxx_std_20)
target_link_libraries(
    99-engine
    PUBLIC SDL3::SDL3-static
           OpenGL::GL
           imgui
           assimp::assimp)

add_executable(99-game game.cpp game.h main.cpp)
target_include_directories(99-game PUBLIC ${CMAKE_CURRENT_LIST_DIR}
                                          ${CMAKE_SOURCE_DIR}/modules)
target_compile_features(99-game PUBLIC cxx_std_20)
target_link_libraries(99-game PRIVATE 99-engine)

if(MSVC)
    set_target_properties(99-game PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
                                             ${CMAKE_SOURCE_DIR})
endif()
