cmake_minimum_required(VERSION 3.26)

project(02-1-sdl-version)

set(SDL3_INCLUDE_DIR /usr/local/include)
set(SDL3_SHARED_LIB /usr/local/lib64/libSDL3.so)
set(SDL3_STATIC_LIB /usr/local/lib64/libSDL3.a)

add_executable(02-1-sdl-version-dynamic main.cpp)
target_include_directories(02-1-sdl-version-dynamic PRIVATE ${SDL3_INCLUDE_DIR})
target_link_libraries(02-1-sdl-version-dynamic PRIVATE ${SDL3_SHARED_LIB})

add_executable(02-1-sdl-version-static main.cpp)
target_include_directories(02-1-sdl-version-static PRIVATE ${SDL3_INCLUDE_DIR})
target_link_libraries(02-1-sdl-version-static PRIVATE ${SDL3_STATIC_LIB})

# find_package(SDL3 COMPONENTS REQUIRED)

# add_executable(02-1-sdl-version-dynamic main.cpp)
# target_link_libraries(02-1-sdl-version-dynamic PRIVATE SDL3::SDL3-shared)


# add_executable(02-1-sdl-version-static main.cpp)
# target_link_libraries(02-1-sdl-version-static PRIVATE SDL3::SDL3-static)