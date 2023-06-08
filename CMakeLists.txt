cmake_minimum_required(VERSION 3.25) 

project(GameDevCourse)

include(CTest)


if ( WIN32 )
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    set(BUILD_SHARED_LIBS ON)
endif()


find_package(SDL3)

if ( (NOT SDL3_FOUND) AND EXISTS "${CMAKE_SOURCE_DIR}/modules/SDL/CMakeLists.txt" )
    message(STATUS "SDL3 doesnt found")
    message(STATUS "Compiling...")


    set(SDL_TEST OFF)
    add_subdirectory(${CMAKE_SOURCE_DIR}/modules/SDL
                    ${CMAKE_BINARY_DIR}/lib/SDL)
    message(STATUS "Compiling SDL3 is succesuful.")
elseif(NOT SDL3_FOUND)
    message(ERROR "Cant find Assimp source, please try execute:\ngit submodule init\ngit submodule update")
endif()

find_package(SDL3 REQUIRED)
message(STATUS "SDL version find: ${SDL3_VERSION_MAJOR}.${SDL3_VERSION_MINOR}.${SDL3_VERSION_PATCH}")

if ( EXISTS "${CMAKE_SOURCE_DIR}/modules/Assimp/CMakeLists.txt" )

    set(ZLIB_LIBRARIES zlibstatic)
    set(ENABLE_BOOST_WORKAROUND OFF)
    set(BUILD_SHARED_LIBS OFF)
    set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
    set(ASSIMP_BUILD_TESTS OFF)
    set(ASSIMP_WARNINGS_AS_ERRORS OFF)
    set(ASSIMP_INSTALL OFF)
    
    set(ASSIMP_LIBRARY_DIRS "${CMAKE_BINARY_DIR}/lib/Assimp/bin/libassimp.so" CACHE INTERNAL "ASSIMP_LIBRARY_DIRS")
    set(ASSIMP_INCLUDE_DIRS "${CMAKE_BINARY_DIR}/lib/Assimp/include/" CACHE INTERNAL "ASSIMP_INCLUDE_DIRS")
    add_subdirectory(${CMAKE_SOURCE_DIR}/modules/Assimp/
                    ${CMAKE_BINARY_DIR}/lib/Assimp)    
    message(STATUS "ASSIMP compiled!")
else()
    message(ERROR "Cant find Assimp source, please try execute:\ngit submodule init\ngit submodule update")
endif()


# add_subdirectory(01-1-hello-world)
# add_subdirectory(01-2-hello-world-lib)
# add_subdirectory(02-1-sdl-version)
# add_subdirectory(03-1-sdl-hot-reload)
# add_subdirectory(04-1-render-basic)
# add_subdirectory(04-2-render-opengl)
# add_subdirectory(04-3-render-morphing)
# add_subdirectory(05-1-texture)

add_subdirectory(99-game)