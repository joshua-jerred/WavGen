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

/**
 * @brief Get the size of a file in bytes. Does not check if the file is open.
 * Returns with the file pointer in the same position.
 * @param file - The file to get the size of.
 * @return uint32_t - The size of the file in bytes.
 */
template <typename stream_t> inline uint32_t getFileSize(stream_t &file) {
  auto current_pos = file.tellg();
  file.seekg(0, std::ios::end);
  uint32_t file_size = file.tellg();
  file.seekg(0, std::ios::beg);
  file.seekg(current_pos);
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

#endif /* FILE_HPP_ */