#include <filesystem>

#include "gtest/gtest.h"

#include "constants.hpp"
#include "file.hpp"
#include "wav_gen.hpp"

const std::string kTestFileName = "test.wav";
const uint32_t kHeaderSize = 44;

class WavFileWriterTest : public ::testing::Test {
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

TEST_F(WavFileWriterTest, GeneratesEmptyWavWithHeader) {
  // SETUP
  wavgen::Writer wav_file(kTestFileName);
  wav_file.done();

  // ASSERT
  ASSERT_TRUE(std::filesystem::exists(kTestFileName));
  ASSERT_EQ(std::filesystem::file_size(kTestFileName), kHeaderSize);
}

TEST_F(WavFileWriterTest, AddSamplesIncreasesFileSize) {
  constexpr uint32_t kNumSamplesToAdd = 100;
  constexpr uint32_t kBytesPerSample = 2;
  constexpr uint32_t kExpectedFileSize =
      kHeaderSize + kNumSamplesToAdd * kBytesPerSample;

  // SETUP
  wavgen::Writer wav_file(kTestFileName);
  int16_t sample = 0;
  for (uint32_t i = 0; i < kNumSamplesToAdd; i++) {
    wav_file.addSample(sample);
  }
  uint32_t num_samples = wav_file.getNumSamples();
  wav_file.done();

  // ASSERT
  ASSERT_TRUE(std::filesystem::exists(kTestFileName));
  ASSERT_EQ(std::filesystem::file_size(kTestFileName), kExpectedFileSize);
  ASSERT_EQ(num_samples, kNumSamplesToAdd);
}

TEST_F(WavFileWriterTest, DurationCalculatedCorrectly) {
  constexpr uint32_t kNumSamplesToAdd = wavgen::kSampleRate;
  constexpr uint32_t kBytesPerSample = 2;
  constexpr uint32_t kExpectedFileSize =
      kHeaderSize + kNumSamplesToAdd * kBytesPerSample;

  // SETUP
  wavgen::Writer wav_file(kTestFileName);
  int16_t sample = 0;
  for (uint32_t i = 0; i < kNumSamplesToAdd; i++) {
    wav_file.addSample(sample);
  }
  uint32_t num_samples = wav_file.getNumSamples();
  uint32_t duration_ms = wav_file.getDuration();
  uint32_t file_size = wav_file.getFileSize();
  wav_file.done();

  // ASSERT
  ASSERT_TRUE(std::filesystem::exists(kTestFileName));
  ASSERT_EQ(std::filesystem::file_size(kTestFileName), kExpectedFileSize);
  ASSERT_EQ(num_samples, kNumSamplesToAdd);
  ASSERT_EQ(duration_ms, 1000);
  ASSERT_EQ(file_size - kHeaderSize, kNumSamplesToAdd * kBytesPerSample);
}

TEST_F(WavFileWriterTest, HeaderSavedCorrectly) {
  constexpr uint32_t kNumSamplesToAdd = wavgen::kSampleRate;
  constexpr uint32_t kBytesPerSample = 2;
  constexpr uint32_t kExpectedFileSize =
      kHeaderSize + kNumSamplesToAdd * kBytesPerSample;

  // SETUP
  wavgen::Writer wav_file(kTestFileName);
  int16_t sample = 0;
  for (uint32_t i = 0; i < kNumSamplesToAdd; i++) {
    wav_file.addSample(sample);
  }
  wav_file.done();

  std::ifstream wav_file_stream(kTestFileName, std::ios::binary);
  wavgen::WavHeader header;
  wav_file_stream >> header;

  // ASSERT
  ASSERT_TRUE(std::filesystem::exists(kTestFileName));
  ASSERT_EQ(std::filesystem::file_size(kTestFileName), kExpectedFileSize);
  ASSERT_EQ(header.data_chunk_size, kNumSamplesToAdd * kBytesPerSample);
  ASSERT_EQ(header.file_size, kExpectedFileSize - 8);
}