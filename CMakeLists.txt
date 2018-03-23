cmake_minimum_required(VERSION 2.8.7)

project(2DFFT)

find_package( OpenCV REQUIRED )

set(2DFFT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(2DFFT_SRC_DIR ${PROJECT_SOURCE_DIR}/src)

include_directories(${2DFFT_INCLUDE_DIR})
include_directories(${2DFFT_SRC_DIR})

add_executable(2dfft
               ${2DFFT_SRC_DIR}/main.cpp
               ${2DFFT_SRC_DIR}/run.cpp)


target_link_libraries(2dfft ${OpenCV_LIBS})


set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fopenmp -std=gnu++17 -Wall -Wextra -pedantic")
