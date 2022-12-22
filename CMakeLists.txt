cmake_minimum_required(VERSION 3.14)
project(WavGen VERSION 1.0 DESCRIPTION "WAV file generator")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ansi -pedantic -Wall -Wextra -Weffc++ -Wdisabled-optimization -Wfloat-equal")


add_library(WavGenlib STATIC
    wavgen.cpp
)

add_executable(example example.cpp)
target_link_libraries(example WavGenlib)
