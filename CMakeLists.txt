cmake_minimum_required(VERSION 3.14)
project(WavGen VERSION 1.1 DESCRIPTION "WAV file generator")

set(MWAV_MAIN_PROJECT OFF)
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(MWAV_MAIN_PROJECT ON)
endif()
message(STATUS "=== MWAV Main project: ${MWAV_MAIN_PROJECT}")

option(WAVGEN_UNIT_TESTS "Enable tests" OFF)
option(WAVGEN_EXAMPLE "Build the example" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Weffc++ -Wdisabled-optimization -Wfloat-equal")

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

if(WAVGEN_UNIT_TESTS OR MWAV_MAIN_PROJECT)
    add_subdirectory(tests)
endif()

if(WAVGEN_EXAMPLE OR MWAV_MAIN_PROJECT)
    add_executable(example example.cpp)
    target_link_libraries(example WavGen)

    # add_executable(wav_stats wav_stats.cpp)
    # target_link_libraries(wav_stats WavGen)
endif()