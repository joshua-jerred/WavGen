#include <filesystem>

#include "gtest/gtest.h"

#include "wav_file.hpp"

const std::string kTestFileName = "test.wav";
const uint32_t kExpectedFileSize = 44;

class WavHeaderTest : public ::testing::Test {
protected:
  void SetUp() override {
    // Delete the file if it exists.
    if (std::filesystem::exists(kTestFileName)) {
      std::filesystem::remove(kTestFileName);
    }
    // Assert that the file does not exist.
    ASSERT_FALSE(std::filesystem::exists(kTestFileName));
  }

  void TearDown() override {
    // Delete the file if it exists.
    if (std::filesystem::exists(kTestFileName)) {
      std::filesystem::remove(kTestFileName);
    }
  }
};

TEST_F(WavHeaderTest, WritesHeader) {
  // Write the header.
  wavgen::WavHeader out_header;
  out_header.file_size = 100;
  out_header.data_chunk_size = 50;
  std::ofstream out_file(kTestFileName);
  out_file << out_header;
  out_file.close();

  // Assert that the file exists and is the correct size.
  ASSERT_TRUE(std::filesystem::exists(kTestFileName));
  ASSERT_EQ(std::filesystem::file_size(kTestFileName), kExpectedFileSize);
}

TEST_F(WavHeaderTest, WritesAndReadsHeader) {
  // Write the header.
  wavgen::WavHeader out_header;
  out_header.file_size = 100;
  out_header.data_chunk_size = 50;
  std::ofstream out_file(kTestFileName);
  out_file << out_header;
  out_file.close();

  // Read the header.
  wavgen::WavHeader in_header;
  std::ifstream in_file(kTestFileName);
  in_file >> in_header;
  in_file.close();

  // Assert that the header values are the same.
  ASSERT_EQ(out_header.file_size, in_header.file_size);
  ASSERT_EQ(out_header.data_chunk_size, in_header.data_chunk_size);

  // Delete the file.
  std::filesystem::remove(kTestFileName);
}

TEST_F(WavHeaderTest, MultipleWritesResultsInSameHeader) {
  // Write the header multiple times.
  wavgen::WavHeader out_header;
  out_header.file_size = 100;
  out_header.data_chunk_size = 50;
  std::ofstream out_file(kTestFileName);

  for (int i = 0; i < 10; i++) {
    out_file << out_header;
  }

  out_header.file_size = 200;
  out_file << out_header;
  out_file.close();

  // Read the header.
  wavgen::WavHeader in_header;
  std::ifstream in_file(kTestFileName);
  in_file >> in_header;
  in_file.close();

  // Assert that the header values are the same.
  ASSERT_EQ(out_header.file_size, in_header.file_size);
  ASSERT_EQ(out_header.data_chunk_size, in_header.data_chunk_size);
}

TEST_F(WavHeaderTest, MultipleReadsDoesNotChangeHeader) {
  // Write the header.
  wavgen::WavHeader out_header;
  out_header.file_size = 100;
  out_header.data_chunk_size = 50;
  std::ofstream out_file(kTestFileName);
  out_file << out_header;
  out_file.close();

  // Read the header.
  wavgen::WavHeader in_header;
  std::ifstream in_file(kTestFileName);
  for (int i = 0; i < 10; i++) {
    in_file >> in_header;
  }
  in_file.close();

  // Assert that the header values are the same.
  ASSERT_EQ(out_header.file_size, in_header.file_size);
  ASSERT_EQ(out_header.data_chunk_size, in_header.data_chunk_size);
}