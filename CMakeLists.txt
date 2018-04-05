cmake_minimum_required(VERSION 3.8)
project(rocket)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        lander.h)

add_executable(rocket ${SOURCE_FILES})
add_executable(lander lander.c)
add_executable(setburnrate setburnrate.cpp)