cmake_minimum_required(VERSION 3.16)

project(01-1-hello-world)

add_executable(01-1-hello-world main.cpp)

add_test(NAME execute-test-base COMMAND 01-1-hello-world)
