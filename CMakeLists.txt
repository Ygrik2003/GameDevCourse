cmake_minimum_required(VERSION 3.22)

project(GameDevCourse)

add_subdirectory(modules)

# add_subdirectory(01-1-hello-world)
# add_subdirectory(01-2-hello-world-lib)
# add_subdirectory(02-1-sdl-version)
# add_subdirectory(03-1-sdl-hot-reload)
# add_subdirectory(04-1-render-basic)
# add_subdirectory(04-2-render-opengl)
# add_subdirectory(04-3-render-morphing)
# add_subdirectory(05-1-texture)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    add_subdirectory(99-game-port)
else()
    add_subdirectory(99-game)
endif()
