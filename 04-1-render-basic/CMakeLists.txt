cmake_minimum_required(VERSION 3.26)

project(04-1-render-basic)

add_executable(04-1-render-basic canvas_basic_main.cpp
               irender/irender.h irender/irender.cpp
               canvas/canvas_basic.h canvas/canvas_basic.cpp)

target_include_directories(04-1-render-basic PUBLIC
                           ${CMAKE_CURRENT_SOURCE_DIR}/canvas
                           ${CMAKE_CURRENT_SOURCE_DIR}/irender)

add_executable(04-2-line-render line_render_main.cpp
               irender/irender.h irender/irender.cpp
               canvas/canvas_basic.h canvas/canvas_basic.cpp
               line_render/line_render.h line_render/line_render.cpp)

target_include_directories(04-2-line-render PUBLIC
                           ${CMAKE_CURRENT_SOURCE_DIR}/canvas
                           ${CMAKE_CURRENT_SOURCE_DIR}/irender
                           ${CMAKE_CURRENT_SOURCE_DIR}/line_render)

add_executable(04-3-triangle-indexed-render triangle_indexed_render_main.cpp
               irender/irender.h irender/irender.cpp
               canvas/canvas_basic.h canvas/canvas_basic.cpp
               line_render/line_render.h line_render/line_render.cpp
               triangle_indexed_render/triangle_indexed_render.h triangle_indexed_render/triangle_indexed_render.cpp)

target_include_directories(04-3-triangle-indexed-render PUBLIC
                           ${CMAKE_CURRENT_SOURCE_DIR}/canvas
                           ${CMAKE_CURRENT_SOURCE_DIR}/irender
                           ${CMAKE_CURRENT_SOURCE_DIR}/line_render
                           ${CMAKE_CURRENT_SOURCE_DIR}/triangle_indexed_render)

add_executable(04-4-triangle-interpolated-render triangle_interpolated_render_main.cpp
               core/matrix.h core/matrix.cpp
               core/view_convertation.h core/view_convertation.cpp             
               irender/irender.h irender/irender.cpp
               canvas/canvas_basic.h canvas/canvas_basic.cpp
               line_render/line_render.h line_render/line_render.cpp
               triangle_indexed_render/triangle_indexed_render.h 
               triangle_indexed_render/triangle_indexed_render.cpp
               triangle_interpolated_render/triangle_interpolated_render.h 
               triangle_interpolated_render/triangle_interpolated_render.cpp)

target_include_directories(04-4-triangle-interpolated-render PUBLIC
                           ${CMAKE_CURRENT_SOURCE_DIR}/canvas
                           ${CMAKE_CURRENT_SOURCE_DIR}/irender
                           ${CMAKE_CURRENT_SOURCE_DIR}/line_render
                           ${CMAKE_CURRENT_SOURCE_DIR}/triangle_indexed_render
                           ${CMAKE_CURRENT_SOURCE_DIR}/triangle_interpolated_render
                           ${CMAKE_CURRENT_SOURCE_DIR}/core)

add_executable(04-5-3d-render 3d_render_main.cpp
               core/matrix.h core/matrix.cpp
               core/view_convertation.h core/view_convertation.cpp             
               irender/irender.h irender/irender.cpp
               canvas/canvas_basic.h canvas/canvas_basic.cpp
               line_render/line_render.h line_render/line_render.cpp
               triangle_indexed_render/triangle_indexed_render.h 
               triangle_indexed_render/triangle_indexed_render.cpp
               triangle_interpolated_render/triangle_interpolated_render.h 
               triangle_interpolated_render/triangle_interpolated_render.cpp)

target_include_directories(04-5-3d-render PUBLIC
                           ${CMAKE_CURRENT_SOURCE_DIR}/canvas
                           ${CMAKE_CURRENT_SOURCE_DIR}/irender
                           ${CMAKE_CURRENT_SOURCE_DIR}/line_render
                           ${CMAKE_CURRENT_SOURCE_DIR}/triangle_indexed_render
                           ${CMAKE_CURRENT_SOURCE_DIR}/triangle_interpolated_render
                           ${CMAKE_CURRENT_SOURCE_DIR}/core)

find_package(SDL3 REQUIRED)
target_link_libraries(04-5-3d-render PUBLIC SDL3::SDL3-shared)