cmake_minimum_required(VERSION 2.8)
project(spatial)

## Check for Windows ##
if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif ()

include_directories(src)
add_subdirectory(src)
# add_subdirectory(qt)

find_library(bcm2835Lib bcm2835)

add_executable(spatial spatial.cpp)
target_link_libraries(spatial adxlLib kalmanLib bcm2835Lib)
