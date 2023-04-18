cmake_minimum_required(VERSION 3.26)

project(03-1-dll-injection)



add_library(03-1-dll-injection SHARED 
            dll_injection.cpp dll_injection.h
            )
target_compile_features(03-1-dll-injection PUBLIC cxx_std_20)

find_package(SDL3 REQUIRED)
find_package(Boost COMPONENTS system filesystem REQUIRED)

target_include_directories(03-1-dll-injection PUBLIC
                           ${Boost_INCLUDE_DIRS}
                           03-1-engine)
                           
target_link_libraries(03-1-dll-injection PRIVATE 
                      SDL3::SDL3 
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY})

