cmake_minimum_required(VERSION 3.22)

project(99-game CXX)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Android")

else()
    find_package(SDL3 REQUIRED)
    find_package(OpenGL REQUIRED)
endif()

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
    99-engine
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
           ${CMAKE_SOURCE_DIR}/modules
           ${CMAKE_SOURCE_DIR}/modules/SDL3/include
           ${CMAKE_SOURCE_DIR}/modules/Assimp/include
           ${CMAKE_BINARY_DIR}/lib/Assimp/include)

target_compile_features(99-engine PUBLIC cxx_std_17)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    target_link_libraries(
        99-engine
        PRIVATE SDL3::SDL3-static
                android
                log
                EGL
                GLESv2
                imgui
                assimp::assimp)
else()
    target_link_libraries(
        99-engine
        PUBLIC SDL3::SDL3-static
               OpenGL::GL
               imgui
               assimp::assimp)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    add_library(99-game SHARED game.cpp game.h main.cpp)
else()
    add_executable(99-game game.cpp game.h main.cpp)
endif()

target_include_directories(
    99-game
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}
           ${CMAKE_SOURCE_DIR}/modules
           ${CMAKE_CURRENT_SOURCE_DIR}
           ${CMAKE_SOURCE_DIR}/modules/Assimp/include)
target_compile_features(99-game PUBLIC cxx_std_17)
target_link_libraries(99-game PRIVATE 99-engine)

if(MINGW)
    add_custom_command(
        TARGET 99-game
        POST_BUILD
        COMMAND
            ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:assimp::assimp>
            $<TARGET_FILE_DIR:99-game>)
endif()

if(MSVC)
    set_target_properties(99-game PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
                                             ${CMAKE_SOURCE_DIR})
endif()
