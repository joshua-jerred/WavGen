/**
 * @file file.hpp
 * @author Joshua Jerred (https://joshuajer.red)
 * @brief Simple file utilities.
 * @date 2023-07-22
 * @copyright Copyright (c) 2023
 */

#ifndef FILE_HPP_
#define FILE_HPP_

#include <fstream>

#include "wav_gen.hpp"

namespace wavgen {

struct WavHeader {
  uint32_t file_size;
  uint32_t data_chunk_size;
};

std::ofstream &operator<<(std::ofstream &out_file, const WavHeader &header);
std::ifstream &operator>>(std::ifstream &in_file, WavHeader &header);

/**
 * @brief Get the size of an input file in bytes.
 *
 * @param file - The input file stream to get the size of.
 * @return uint32_t - The size of the file in bytes.
 */
inline uint32_t calculateFileSize(std::ifstream &file) {
  auto current_pos = file.tellg();
  file.seekg(0, std::ios::end);
  uint32_t file_size = file.tellg();
  file.seekg(0, std::ios::beg);
  file.seekg(current_pos);
  return file_size;
}

/**
 * @brief Get the size of an output file in bytes.
 *
 * @param file - The output file stream to get the size of.
 * @return uint32_t - The size of the file in bytes.
 */
inline uint32_t calculateFileSize(std::ofstream &file) {
  auto current_pos = file.tellp();
  file.seekp(0, std::ios::end);
  uint32_t file_size = file.tellp();
  file.seekp(0, std::ios::beg);
  file.seekp(current_pos);
  return file_size;
}

/**
 * @brief Write 1 to 4 bytes to a file.
 *
 * @param wav_file - The file to write to.
 * @param data - The data to write.
 * @param num_bytes - The number of bytes to write.
 */
template <uint8_t bytes_to_write>
inline void writeBytes(std::ofstream &wav_file, uint32_t data) {
  static_assert(bytes_to_write <= 4 && bytes_to_write > 0,
                "Invalid number of bytes to write");
  wav_file.write(reinterpret_cast<const char *>(&data), bytes_to_write);
}

/**
 * @brief Validate that a file is open.
 *
 * @tparam stream_t - The type of the file stream.
 * @param wav_file - The file to validate.
 */
template <typename stream_t> inline void validateFileOpen(stream_t &wav_file) {
  if (!wav_file.is_open()) {
    throw std::runtime_error("File is not open");
  }
}

template <typename stream_t>
inline uint32_t calculateDataChunkSize(stream_t &file) {
  auto file_size = calculateFileSize(file);
  auto data_chunk_size = file_size - HEADER_SIZE;
  return data_chunk_size;
}

template <typename stream_t>
inline uint32_t calculateNumberOfSamples(stream_t &file) {
  return calculateDataChunkSize(file) / 2;
}

template <typename stream_t>
inline uint32_t calculateHeaderFileSize(stream_t &file) {
  constexpr uint32_t kSizeOffset = 8;
  auto file_size = calculateFileSize(file);
  return file_size - kSizeOffset;
}

/**
 * @brief Calculate the duration of a WAV file in milliseconds.
 *
 * @tparam stream_t - The type of the file stream.
 * @param file - The file to calculate the duration of.
 * @return uint32_t - The duration of the WAV file in milliseconds.
 */
template <typename stream_t> inline uint32_t calculateDuration(stream_t &file) {
  auto num_samples = calculateNumberOfSamples(file);
  constexpr uint32_t kSamplesPerMillisecond = SAMPLE_RATE / 1000;
  return num_samples / kSamplesPerMillisecond;
}

} // namespace wavgen

#endif /* FILE_HPP_ */