cmake_minimum_required(VERSION 3.20)

project(99-game CXX)


add_library(imgui STATIC 
            imgui/imconfig.h 
            imgui/imgui.h imgui/imgui.cpp
            imgui/imstb_rectpack.h imgui/imgui_draw.cpp
            imgui/imgui_widgets.cpp
            imgui/imgui_demo.cpp imgui/imgui_tables.cpp
            imgui/imgui_internal.h imgui/imstb_textedit.h
            imgui/imstb_truetype.h
            )
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_property(TARGET imgui PROPERTY POSITION_INDEPENDENT_CODE ON)


find_package(OpenGL REQUIRED)

add_library(99-engine STATIC
            glad/glad.h glad/glad.c
            KHR/khrplatform.h
            engine/engine.h engine/engine.cpp
            engine/shader_opengl.h engine/shader_opengl.cpp
            engine/texture_opengl.h engine/texture_opengl.cpp
            engine/vertex_buffer.h engine/vertex_buffer.cpp
            engine/index_buffer.h engine/index_buffer.cpp
            core/config.h core/event.h
            core/types.h core/types.cpp 
            objects/figure.h objects/figure.cpp 
            objects/camera.h objects/camera.cpp
            objects/mesh.h objects/mesh.cpp
            objects/model.h objects/model.cpp
            objects/object.h objects/object.cpp
            core/picopng.hxx)
target_include_directories(99-engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(99-engine PUBLIC cxx_std_20)
target_link_libraries(99-engine SDL3::SDL3-static OpenGL::GL imgui assimp::assimp)





add_executable(99-game main.cpp game.h game.cpp)
target_include_directories(99-game PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_compile_features(99-game PUBLIC cxx_std_20)
target_link_libraries(99-game PRIVATE 99-engine)