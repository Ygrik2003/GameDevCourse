cmake_minimum_required(VERSION 3.16)

project(hello-world-bin)


add_executable(bin-static src/main.cpp)
target_compile_features(bin-static PRIVATE cxx_std_20)
target_link_libraries(bin-static LINK_PUBLIC hello-lib-static)
target_link_options(bin-static PRIVATE -static)


add_executable(bin-dynamic src/main.cpp)
target_compile_features(bin-dynamic PRIVATE cxx_std_20)
target_link_libraries(bin-dynamic LINK_PUBLIC hello-lib-dynamic)

