#include <filesystem>

#include "gtest/gtest.h"

// #include "wav_file.hpp"

// TEST(WavFileTest, GeneratesEmptyWavWithHeader) {
//   const std::string kOutputFilename = "test.wav";
//   constexpr uint32_t kExpectedFileSize = 44;
//   // delete the file if it exists
//   if (std::filesystem::exists(kOutputFilename)) {
//     std::filesystem::remove(kOutputFilename);
//   }

//   // SETUP
//   wavgen::WavFile wav_file(kOutputFilename);
//   wav_file.done();

//   // ASSERT
//   ASSERT_TRUE(std::filesystem::exists(kOutputFilename));
//   ASSERT_EQ(std::filesystem::file_size(kOutputFilename), kExpectedFileSize);
// }

// TEST(WavFileTest, AddSamplesIncreasesFileSize) {
//   const std::string kOutputFilename = "test.wav";
//   constexpr uint32_t kNumSamplesToAdd = 100;
//   constexpr uint32_t kBytesPerSample = 2;
//   constexpr uint32_t kExpectedFileSize =
//       44 + kNumSamplesToAdd * kBytesPerSample;
//   // delete the file if it exists
//   if (std::filesystem::exists(kOutputFilename)) {
//     std::filesystem::remove(kOutputFilename);
//   }

//   // SETUP
//   wavgen::WavFile wav_file(kOutputFilename);
//   for (uint32_t i = 0; i < kNumSamplesToAdd; i++) {
//     wav_file.addSample(0);
//   }
//   wav_file.done();

//   // ASSERT
//   ASSERT_TRUE(std::filesystem::exists(kOutputFilename));
//   ASSERT_EQ(std::filesystem::file_size(kOutputFilename), kExpectedFileSize);
// }