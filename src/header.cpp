/**
 * @file header.cpp
 * @author Joshua Jerred (https://joshuajer.red)
 * @brief Functions for reading and writing WAV headers.
 * @date 2023-07-22
 * @copyright Copyright (c) 2023
 */

#include <filesystem>

#include "constants.hpp"
#include "file.hpp"
#include "wav_gen.hpp"

namespace wavgen {

inline constexpr uint32_t kWavHeaderSize = 44;

// RIFF****WAVEfmt
const std::string kRiffChunkDescriptor = "RIFF";
const std::string kWavFormat = "WAVE";
const std::string kFormatChunkDescriptor = "fmt ";

inline constexpr uint32_t kFormatChunkSize = 16;
inline constexpr uint32_t kFormatCode = 1;
inline constexpr uint32_t kNumChannels = 1;

inline constexpr uint32_t kByteRate =
    (kSampleRate * kBitsPerSample * kNumChannels) / 8;
inline constexpr uint32_t kBlockAlign = (kBitsPerSample * kNumChannels) / 8;
const std::string kDataChunkDescriptor = "data";

std::ofstream &operator<<(std::ofstream &out_file, const WavHeader &header) {
  if (!out_file.is_open()) {
    throw std::runtime_error("Failed to write header. File not open.");
  }

  // keep track of the initial position so we can jump back to it later.
  const auto initial_position = out_file.tellp();

  // Jump to the beginning of the file.
  out_file.seekp(0, std::ios::beg);

  // Write the header data.
  out_file << kRiffChunkDescriptor;
  writeBytes<4>(out_file, header.file_size);
  out_file << kWavFormat;
  out_file << kFormatChunkDescriptor;
  writeBytes<4>(out_file, kFormatChunkSize);
  writeBytes<2>(out_file, kFormatCode);
  writeBytes<2>(out_file, kNumChannels);
  writeBytes<4>(out_file, kSampleRate);
  writeBytes<4>(out_file, kByteRate);
  writeBytes<2>(out_file, kBlockAlign);
  writeBytes<2>(out_file, kBitsPerSample);
  out_file << kDataChunkDescriptor;
  writeBytes<4>(out_file, header.data_chunk_size);

  // Jump back to the initial position.
  out_file.seekp(initial_position);
  return out_file;
}

std::ifstream &operator>>(std::ifstream &in_file, WavHeader &header) {
  if (!in_file.is_open()) {
    throw std::runtime_error("Failed to read header. File not open.");
  }

  // Ensure that the file is large enough to contain a header.
  const uint32_t file_size = calculateFileSize(in_file);
  if (file_size < kWavHeaderSize) {
    throw std::runtime_error("Failed to read header. File is too small.");
  }

  // Read the header data into a buffer.
  in_file.seekg(0, std::ios::beg); // Jump to the beginning of the file.
  std::array<char, kWavHeaderSize> header_data;
  in_file.read(header_data.data(), kWavHeaderSize);

  // Check the RIFF chunk descriptor.
  const std::string riff_chunk_descriptor =
      std::string(header_data.begin(), header_data.begin() + 4);
  if (riff_chunk_descriptor != kRiffChunkDescriptor) {
    throw std::runtime_error("Failed to read header. Invalid RIFF chunk.");
  }

  // Read the size of the overall file.
  header.file_size = *reinterpret_cast<uint32_t *>(&header_data[4]);

  // Check the WAV format.
  const std::string wav_format =
      std::string(header_data.begin() + 8, header_data.begin() + 12);
  if (wav_format != kWavFormat) {
    throw std::runtime_error("Failed to read header. Invalid WAV format.");
  }

  // Check the format chunk descriptor.
  const std::string format_chunk_descriptor =
      std::string(header_data.begin() + 12, header_data.begin() + 16);
  if (format_chunk_descriptor != kFormatChunkDescriptor) {
    throw std::runtime_error("Failed to read header. Invalid format chunk.");
  }

  // Check the format chunk size.
  const uint32_t format_chunk_size =
      *reinterpret_cast<uint32_t *>(&header_data[16]);
  if (format_chunk_size != kFormatChunkSize) {
    throw std::runtime_error(
        "Failed to read header. Invalid format chunk size.");
  }

  // Check the format code.
  const uint16_t format_code = *reinterpret_cast<uint16_t *>(&header_data[20]);
  if (format_code != kFormatCode) {
    throw std::runtime_error("Failed to read header. Invalid format code.");
  }

  // Check the number of channels.
  const uint16_t num_channels = *reinterpret_cast<uint16_t *>(&header_data[22]);
  if (num_channels != kNumChannels) {
    throw std::runtime_error("Failed to read header. Invalid number of "
                             "channels. Only mono files are supported.");
  }

  // Read the sample rate.
  const uint32_t sample_rate = *reinterpret_cast<uint32_t *>(&header_data[24]);
  if (sample_rate != kSampleRate) {
    throw std::runtime_error("Failed to read header. Invalid sample rate.");
  }

  // Read the byte rate.
  const uint32_t byte_rate = *reinterpret_cast<uint32_t *>(&header_data[28]);
  if (byte_rate != kByteRate) {
    throw std::runtime_error("Failed to read header. Invalid byte rate.");
  }

  // Read the block align.
  const uint16_t block_align = *reinterpret_cast<uint16_t *>(&header_data[32]);
  if (block_align != kBlockAlign) {
    throw std::runtime_error("Failed to read header. Invalid block align.");
  }

  // Read the bits per sample.
  const uint16_t bits_per_sample =
      *reinterpret_cast<uint16_t *>(&header_data[34]);
  if (bits_per_sample != kBitsPerSample) {
    throw std::runtime_error("Failed to read header. Invalid bits per sample.");
  }

  // Check the data chunk descriptor.
  const std::string data_chunk_descriptor =
      std::string(header_data.begin() + 36, header_data.begin() + 40);
  if (data_chunk_descriptor != kDataChunkDescriptor) {
    throw std::runtime_error("Failed to read header. Invalid data chunk.");
  }

  // Read the data chunk size.
  header.data_chunk_size = *reinterpret_cast<uint32_t *>(&header_data[40]);

  return in_file;
}

} // namespace wavgen