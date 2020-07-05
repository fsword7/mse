cmake_minimum_required (VERSION 3.17)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED YES)

find_package(fmt REQUIRED)

project(mse)

include_directories(src)

add_subdirectory(src)
