/**
 * @file wav_file.cpp
 * @author Joshua Jerred (https://joshuajer.red)
 * @brief A simple WAV file structure (16-bit, single channel)
 * @date 2023-07-21
 * @copyright Copyright (c) 2023
 */

#include "file.hpp"
#include "wav_gen.hpp"

#include <algorithm>

namespace wavgen {

Writer::Writer(std::string output_filename) {
  wav_file_.open(output_filename, std::ios::binary);
  validateFileOpen(wav_file_);

  WavHeader header;
  wav_file_ << header; // Write the header to the file to reserve space
  wav_file_.seekp(0, std::ios::end); // Jump to the end of the file
}

Writer::~Writer() {
  if (wav_file_.is_open()) {
    done();
  }
}

uint32_t Writer::getNumSamples() {
  return calculateNumberOfSamples(wav_file_);
}

uint32_t Writer::getDuration() {
  return calculateDuration(wav_file_);
}

uint32_t Writer::getFileSize() {
  return calculateFileSize(wav_file_);
}

void Writer::addSample(double sample) {
  std::clamp(sample, -1.0, 1.0);
  int16_t sample_int = static_cast<int16_t>(sample * kMaxAmplitude);
  writeBytes<2>(wav_file_, sample_int);
}

void Writer::addSample(int16_t sample) {
  writeBytes<2>(wav_file_, sample);
}

void Writer::done() {
  validateFileOpen(wav_file_);
  WavHeader header;
  header.data_chunk_size = calculateDataChunkSize(wav_file_);
  header.file_size = calculateHeaderFileSize(wav_file_);
  wav_file_ << header;
  wav_file_.close();
}

} // namespace wavgen