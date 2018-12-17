cmake_minimum_required(VERSION 2.8)
project(spatial)

## Check for Windows ##
if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif ()

add_library(kalmanLib kalman_.hpp)

set_target_properties(kalmanLibrary PROPERTIES LINKER_LANGUAGE CXX)
