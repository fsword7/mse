cmake_minimum_required (VERSION 3.21)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED YES)

project(mse)

find_package(SDL2 REQUIRED)
find_package(fmt REQUIRED)
find_package(LibLZMA REQUIRED)

include_directories(src)

add_subdirectory(src)
