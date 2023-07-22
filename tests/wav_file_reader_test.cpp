#include <filesystem>

#include "gtest/gtest.h"

#include "constants.hpp"
#include "wav_gen.hpp"

const std::string kTestFileName = "test.wav";
const uint32_t kHeaderSize = 44;

class WavFileReaderTest : public ::testing::Test {
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

TEST_F(WavFileReaderTest, ReadsEmptyWavWithHeader) {
  // SETUP
  wavgen::Writer writer(kTestFileName);
  writer.done();

  // ASSERT
  wavgen::Reader reader(kTestFileName);
  ASSERT_EQ(reader.getNumSamples(), 0);
  ASSERT_EQ(reader.getDuration(), 0);
  ASSERT_EQ(reader.getFileSize(), kHeaderSize);
}

TEST_F(WavFileReaderTest, ReadsHeaderCorrectly) {
  constexpr uint32_t kNumSamplesToAdd = 100;
  constexpr uint32_t kBytesPerSample = 2;
  constexpr uint32_t kExpectedFileSize =
      kHeaderSize + kNumSamplesToAdd * kBytesPerSample;

  // SETUP
  wavgen::Writer writer(kTestFileName);
  int16_t sample = 0;
  for (uint32_t i = 0; i < kNumSamplesToAdd; i++) {
    writer.addSample(sample);
  }
  writer.done();

  wavgen::Reader reader(kTestFileName);

  // ASSERT
  ASSERT_EQ(reader.getNumSamples(), kNumSamplesToAdd);
  ASSERT_EQ(reader.getFileSize(), kExpectedFileSize);
}

TEST_F(WavFileReaderTest, DurationCalculatedCorrectly) {
  constexpr uint32_t kNumSamplesToAdd = wavgen::kSampleRate;
  constexpr uint32_t kBytesPerSample = 2;
  constexpr uint32_t kExpectedFileSize =
      kHeaderSize + kNumSamplesToAdd * kBytesPerSample;

  // SETUP
  wavgen::Writer writer(kTestFileName);
  int16_t sample = 0;
  for (uint32_t i = 0; i < kNumSamplesToAdd; i++) {
    writer.addSample(sample);
  }
  writer.done();

  wavgen::Reader reader(kTestFileName);
  uint32_t num_samples = reader.getNumSamples();
  uint32_t duration_ms = reader.getDuration();
  uint32_t file_size = reader.getFileSize();

  // ASSERT
  ASSERT_EQ(num_samples, kNumSamplesToAdd);
  ASSERT_EQ(duration_ms, 1000);
  ASSERT_EQ(file_size - kHeaderSize, kNumSamplesToAdd * kBytesPerSample);
  ASSERT_EQ(file_size, kExpectedFileSize);
}

TEST_F(WavFileReaderTest, GetAllSamples) {
  const std::vector<int16_t> kTestSamples = {
      0, 1, 2000, -2000, 32767, -32768, 0, 0, 0, 0, 0, 5, 8, 1, 11, -1};

  // SETUP
  wavgen::Writer writer(kTestFileName);
  for (auto sample : kTestSamples) {
    writer.addSample(sample);
  }
  writer.done();

  std::vector<int16_t> samples;
  wavgen::Reader reader(kTestFileName);
  ASSERT_EQ(reader.getNumSamples(), kTestSamples.size());
  reader.getAllSamples(samples);

  // ASSERT
  EXPECT_EQ(samples.size(), kTestSamples.size());
  for (uint32_t i = 0; i < samples.size(); i++) {
    EXPECT_EQ(samples[i], kTestSamples[i]) << "Sample " << i << " is incorrect";
  }
}