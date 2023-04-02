cmake_minimum_required(VERSION 3.16)

#Add hello world lib
add_library(hello-lib-static STATIC include/hello_world.h src/hello_world.cpp)
target_include_directories(hello-lib-static
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


add_library(hello-lib-dynamic SHARED include/hello_world.h src/hello_world.cpp)
target_include_directories(hello-lib-dynamic
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)



add_library(hello-async-lib-dynamic SHARED include/hello_world_async.h src/hello_world_async.cpp)
target_include_directories(hello-async-lib-dynamic
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


#Boost include
set(Boost_USE_DEBUG_LIBS         ON)
set(Boost_USE_RELEASE_LIBS       ON)
set(Boost_USE_MULTITHREADED      ON)

find_package(Boost 1.66.0 COMPONENTS thread system)


if(Boost_FOUND)
    set(Boost_USE_STATIC_LIBS       OFF)
    set(Boost_USE_STATIC_RUNTIME    OFF)

    include_directories(${Boost_INCLUDE_DIRS})
    
    target_include_directories(hello-async-lib-dynamic PUBLIC
                          ${Boost_INCLUDE_DIRS}
    )
    target_link_libraries(hello-async-lib-dynamic LINK_PUBLIC
                          -lboost_thread
    )
    
else()
    message(FATAL_ERROR "Error: find_package(...) did not found boost libs")
endif()


target_compile_features(hello-lib-static PUBLIC cxx_std_20)
target_compile_features(hello-lib-dynamic PUBLIC cxx_std_20)
target_compile_features(hello-async-lib-dynamic PUBLIC cxx_std_20)

install(TARGETS hello-lib-static hello-lib-dynamic)