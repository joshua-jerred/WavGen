#include <filesystem>

#include "gtest/gtest.h"

#include "file.hpp"
#include "wav_gen.hpp"

const std::string kTestFileName = "test.wav";
const uint32_t HEADER_SIZE = 44;

class WavGeneratorTest : public ::testing::Test {
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

TEST_F(WavGeneratorTest, AddSineWaveDuration) {
  // SETUP
  constexpr uint16_t kFrequency = 410;
  constexpr float kAmplitude = 0.5f;
  constexpr uint16_t kDuration = 1500;

  constexpr uint32_t kExpectedNumSamples =
      kDuration * wavgen::SAMPLE_RATE / 1000;
  constexpr uint32_t kExpectedFileSize = kExpectedNumSamples * 2 + HEADER_SIZE;

  wavgen::Generator wav_file(kTestFileName);
  wav_file.addSineWave(kFrequency, kAmplitude, kDuration);

  uint32_t writer_num_samples = wav_file.getNumSamples();
  uint32_t writer_duration = wav_file.getDuration();

  wav_file.done();

  // ASSERT
  ASSERT_TRUE(std::filesystem::exists(kTestFileName));
  ASSERT_EQ(std::filesystem::file_size(kTestFileName), kExpectedFileSize);
  ASSERT_EQ(writer_num_samples, writer_duration * wavgen::SAMPLE_RATE / 1000);
  ASSERT_EQ(writer_duration, kDuration);
}

TEST_F(WavGeneratorTest, AddSineWaveSamples) {
  // SETUP
  constexpr uint16_t kFrequency = 410;
  constexpr float kAmplitude = 0.5f;
  constexpr uint16_t kNumSamples = 1000;
  constexpr uint32_t kExpectedFileSize = kNumSamples * 2 + HEADER_SIZE;

  wavgen::Generator wav_file(kTestFileName);
  wav_file.addSineWaveSamples(kFrequency, kAmplitude, kNumSamples);

  uint32_t writer_num_samples = wav_file.getNumSamples();

  wav_file.done();

  // ASSERT
  ASSERT_TRUE(std::filesystem::exists(kTestFileName));
  ASSERT_EQ(std::filesystem::file_size(kTestFileName), kExpectedFileSize);
  ASSERT_EQ(writer_num_samples, kNumSamples);
}