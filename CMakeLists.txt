cmake_minimum_required(VERSION 3.8) 

project(GameDevCourse)

include(CTest)


if ( WIN32 )
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

add_subdirectory(01-1-hello-world)
add_subdirectory(01-2-hello-world-lib)

add_subdirectory(02-1-sdl-version)