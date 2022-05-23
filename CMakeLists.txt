cmake_minimum_required(VERSION 3.10)
project(ParentChildProcess)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-pthread")

add_executable(parent Parent.cpp Parent.h)
add_executable(child Child.cpp Child.h)

