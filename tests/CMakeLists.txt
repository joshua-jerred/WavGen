enable_testing()
find_package(GTest REQUIRED)

add_executable(wavgen_unit_tests
  header_test.cpp
  wav_file_writer_test.cpp
  wav_file_reader_test.cpp
  generator_test.cpp
  ${SRC}/wav_file_reader.cpp
  ${SRC}/wav_file_writer.cpp
  ${SRC}/generator.cpp
  ${SRC}/header.cpp
)
target_link_libraries(wavgen_unit_tests GTest::GTest GTest::Main)
target_include_directories(wavgen_unit_tests PRIVATE ${SRC} ${INC})