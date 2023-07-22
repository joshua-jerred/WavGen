/**
 * @file wav_file_reader.cpp
 * @author Joshua Jerred (https://joshuajer.red)
 * @brief A simple WAV file structure (16-bit, single channel)
 * @date 2023-07-21
 * @copyright Copyright (c) 2023
 */

#include "file.hpp"
#include "wav_file.hpp"

namespace wavgen {

Reader::Reader(std::string input_file_path) {
  wav_file_.open(input_file_path, std::ios::binary);
  validateFileOpen(wav_file_);

  WavHeader header;
  wav_file_ >> header; // Read the header from the file

  // Validate the header
  uint32_t expected_num_samples = header.data_chunk_size / 2;
  if (expected_num_samples > getNumSamples()) {
    throw std::runtime_error("More samples in header than can exist in file.");
  }
}

uint32_t Reader::getNumSamples() {
  return calculateNumberOfSamples(wav_file_);
}

uint32_t Reader::getDuration() {
  return calculateDuration(wav_file_);
}

uint32_t Reader::getFileSize() {
  return calculateFileSize(wav_file_);
}

void Reader::getAllSamples(std::vector<int16_t> &samples) {
  const uint32_t num_samples = getNumSamples();
  samples.clear();
  samples.reserve(num_samples);

  // Jump to the beginning of the data chunk.
  wav_file_.seekg(kHeaderSize, std::ios::beg);

  int16_t sample = 0;
  for (uint32_t i = 0; i < num_samples; i++) {
    wav_file_.read(reinterpret_cast<char *>(&sample), 2);
    samples.push_back(sample);
  }
}

} // namespace wavgen