cmake_minimum_required(VERSION 3.16)

add_executable(01-2-hello-world-static src/main.cpp)
target_compile_features(01-2-hello-world-static PRIVATE cxx_std_20)
target_link_libraries(01-2-hello-world-static LINK_PUBLIC hello-lib-static)
target_link_options(01-2-hello-world-static PRIVATE -static)


add_executable(01-2-hello-world-dynamic src/main.cpp)
target_compile_features(01-2-hello-world-dynamic PRIVATE cxx_std_20)
target_link_libraries(01-2-hello-world-dynamic LINK_PUBLIC hello-lib-dynamic)


if (WIN32)
    set_target_properties(hello-lib-dynamic PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

if (Boost_EXAMPLE)
    add_executable(bin-async-dynamic src/main_async.cpp)
    target_compile_features(bin-async-dynamic PRIVATE cxx_std_20)
    target_link_libraries(bin-async-dynamic LINK_PUBLIC hello-async-lib-dynamic)
endif()
