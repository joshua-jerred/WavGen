enable_testing()
find_package(GTest REQUIRED)

add_executable(wavgen_unit_tests
  wav_file_test.cpp
  header_test.cpp
  ${SRC}/wav_file_reader.cpp
  ${SRC}/wav_file_writer.cpp
  ${SRC}/header.cpp
)
target_link_libraries(wavgen_unit_tests GTest::GTest GTest::Main)
target_include_directories(wavgen_unit_tests PRIVATE ${SRC})