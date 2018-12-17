cmake_minimum_required(VERSION 2.8)
project(spatial)

## Check for Windows ##
if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif ()

add_library(adxlLib ADXL345_threaded.h circularfifo_memory_sequential_consistent.hpp)
