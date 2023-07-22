cmake_minimum_required(VERSION 3.14)
project(WavGen VERSION 1.0 DESCRIPTION "WAV file generator")

option(WAVGEN_UNIT_TESTS "Enable tests" OFF)
option(WAVGEN_EXAMPLE "Build the example" ON)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ansi -pedantic -Wall -Wextra -Weffc++ -Wdisabled-optimization -Wfloat-equal")

set(SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC ${CMAKE_CURRENT_SOURCE_DIR}/include)


add_library(WavGen STATIC
    ${SRC}/wav_file_reader.cpp
    ${SRC}/wav_file_writer.cpp
    ${SRC}/generator.cpp
    ${SRC}/header.cpp
)
target_include_directories(WavGen
    PUBLIC ${INC}
    PRIVATE ${SRC}
)

if(WAVGEN_UNIT_TESTS)
    add_subdirectory(tests)
endif()


if(WAVGEN_EXAMPLE)
    add_executable(example example.cpp)
    target_link_libraries(example WavGen)
endif()