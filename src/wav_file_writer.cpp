/**
 * @file wav_file.cpp
 * @author Joshua Jerred (https://joshuajer.red)
 * @brief A simple WAV file structure (16-bit, single channel)
 * @date 2023-07-21
 * @copyright Copyright (c) 2023
 */

#include "wav_file.hpp"

#include <algorithm>

namespace wavgen {

WavFile::WavFile(std::string output_filename) {
  wav_file_.open(output_filename, std::ios::binary);
  validateFileOpen(wav_file_);

  wav_file_ << kRiffChunkDescriptor;          // RIFF header
  writeBytes(wav_file_, kFormatChunkSize, 4); // size
  writeBytes(wav_file_, kFormatCode, 2);      // compression code
  writeBytes(wav_file_, kNumChannels, 2);     // number of channels
  writeBytes(wav_file_, kSampleRate, 4);      // sample rate
  writeBytes(wav_file_, kByteRate, 4);        // Byte rate
  writeBytes(wav_file_, kBlockAlign, 2);      // block align
  writeBytes(wav_file_, kBitsPerSample, 2);   // bits per sample
  wav_file_ << kDataChunkDescriptor;          // data chunk header (data****)

  data_start_ = wav_file_.tellp(); // Save the position of the start of the
                                   // data chunk
}

WavFile::WavFile(std::string output_file_path, std::string input_file_path) {
  wav_file_.open(output_file_path, std::ios::binary);
  validateFileOpen(wav_file_);

  std::ifstream input_file(input_file_path, std::ios::binary);

  wav_file_ << input_file.rdbuf();

  input_file.close();

  data_start_ = wav_file_.tellp(); // Save the position of the start of the
                                   // data chunk
}

WavFile::~WavFile() {
  if (wav_file_.is_open()) {
    done();
  }
}

void WavFile::addSample(double sample) {
  std::clamp(sample, -1.0, 1.0);
  int32_t sample_int = static_cast<int32_t>(sample * kMaxAmplitude);
  writeBytes(wav_file_, sample_int, 2);
}

void WavFile::addSample(int16_t sample) {
  writeBytes(wav_file_, sample, 2);
}

void WavFile::done() {
  validateFileOpen(wav_file_);

  // Get the position of the end of the data chunk
  wav_file_.seekp(0, std::ios::end);
  auto data_end = wav_file_.tellp();

  // Calculate the size of the data chunk
  auto data_size = data_end - data_start_;

  // Go to where the data chunk size is stored (data**** - 4 bytes to overwrite)
  const std::streamoff kDataSizeOffsetFromStart = 4;
  auto data_size_pos = data_start_ - kDataSizeOffsetFromStart;
  wav_file_.seekp(data_size_pos);
  writeBytes(wav_file_, data_size, 4); // Write the size of the data chunk

  // Calculate the size of the overall file (-8 because of the RIFF header)
  const std::streamoff kRiffHeaderSize = 8;
  auto file_size = data_end - kRiffHeaderSize;
  const std::streamoff kFileSizeOffsetFromStart = 4;
  wav_file_.seekp(kFileSizeOffsetFromStart, std::ios::beg);
  writeBytes(wav_file_, file_size, 4); // Write the size of the overall file

  wav_file_.close();
}

} // namespace wavgen