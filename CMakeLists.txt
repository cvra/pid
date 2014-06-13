cmake_minimum_required(VERSION 2.6)

project(pid-filter)
set(CMAKE_BUILD_TYPE Debug)

add_executable(
    tests
    tests/main.cpp
    tests/pid_test.cpp  
    pid.c
    )


target_link_libraries(
    tests
    m
    CppUTest
    CppUTestExt
    )
