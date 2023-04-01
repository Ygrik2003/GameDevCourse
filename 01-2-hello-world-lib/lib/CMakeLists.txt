cmake_minimum_required(VERSION 3.16)

project(hello-lib CXX)

add_library(hello-lib-static STATIC include/hello_world.h src/hello_world.cpp)
target_include_directories(hello-lib-static
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(hello-lib-static PUBLIC cxx_std_20)

add_library(hello-lib-dynamic SHARED include/hello_world.h src/hello_world.cpp)
target_include_directories(hello-lib-dynamic
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(hello-lib-dynamic PUBLIC cxx_std_20)



install(TARGETS hello-lib-static hello-lib-dynamic)