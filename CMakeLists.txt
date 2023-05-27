cmake_minimum_required(VERSION 3.25) 

project(GameDevCourse)

include(CTest)


if ( WIN32 )
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    set(BUILD_SHARED_LIBS ON)
endif()


find_package(SDL)
if ( NOT SDL_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/modules/SDL/CMakeLists.txt" )
add_subdirectory(${CMAKE_SOURCE_DIR}/modules/SDL
                 ${CMAKE_BINARY_DIR}/lib/SDL)
endif()



# if ( EXISTS "${CMAKE_SOURCE_DIR}/modules/Assimp/CMakeLists.txt" )
# 
# set(ZLIB_LIBRARIES zlibstatic)
# set(ENABLE_BOOST_WORKAROUND ON)
# set(BUILD_STATIC_LIB ON)
# set(BUILD_ASSIMP_TOOLS  ON)
# set(ASSIMP_BUILD_STATIC_LIB ON)
# set(ASSIMP_WARNINGS_AS_ERRORS OFF)
# 
# add_subdirectory(${CMAKE_SOURCE_DIR}/modules/Assimp
#                  ${CMAKE_BINARY_DIR}/lib/Assimp)
# endif()


# add_subdirectory(01-1-hello-world)
# add_subdirectory(01-2-hello-world-lib)
add_subdirectory(02-1-sdl-version)
# add_subdirectory(03-1-sdl-hot-reload)
# add_subdirectory(04-1-render-basic)
# add_subdirectory(04-2-render-opengl)
# add_subdirectory(04-3-render-morphing)
# add_subdirectory(05-1-texture)

# add_subdirectory(99-game)
