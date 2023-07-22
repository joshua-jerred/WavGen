/**
 * @file wav_file.hpp
 * @author Joshua Jerred (https://joshuajer.red)
 * @brief A simple WAV file structure (16-bit, single channel)
 * @date 2023-07-21
 * @copyright Copyright (c) 2023
 */

#ifndef WAV_FILE_HPP_
#define WAV_FILE_HPP_

#include "constants.hpp"
#include "file.hpp"

namespace wavgen {

struct WavHeader {
  uint32_t file_size;
  uint32_t data_chunk_size;
};

std::ofstream &operator<<(std::ofstream &out_file, const WavHeader &header);
std::ifstream &operator>>(std::ifstream &in_file, WavHeader &header);

class WavFile {
public:
  WavFile() = default;
  virtual ~WavFile() = default;

  /**
   * @brief Returns the sample rate of the WAV file.
   * @return uint32_t - The sample rate.
   */
  uint32_t getSampleRate() const {
    return kSampleRate;
  }

  /**
   * @brief Get the number of bits per sample.
   * @return uint32_t - The number of bits per sample.
   */
  uint32_t getBitsPerSample() const {
    return kBitsPerSample;
  }

  /**
   * @brief Get the size of the data chunk in bytes.
   *
   * @return uint32_t - The size of the data chunk in bytes.
   */
  virtual uint32_t getDataChunkSize();

  /**
   * @brief Get the number of samples in the WAV file.
   * @return uint32_t - The number of samples in the WAV file.
   */
  virtual uint32_t getNumSamples();

  /**
   * @brief Get the duration of the WAV file in milliseconds.
   * @return uint32_t - The duration in milliseconds.
   */
  virtual uint32_t getDuration();

  /**
   * @brief Get the size of the WAV file in bytes.
   * @return uint32_t - The size of the WAV file in bytes.
   */
  virtual uint32_t getFileSize();
};

class WavFileWriter : public WavFile {
public:
  /**
   * @brief Open a WAV file for writing.
   *
   * @param output_file_path - The name of the file to write to.
   */
  WavFileWriter(std::string output_file_path);

  /**
   * @brief Deconstructor for the WAV file. This will call done().
   */
  ~WavFileWriter();

  /**
   * @brief Add a sample to the WAV file. This is a fast operation.
   *
   * @param sample - A 16-bit signed sample to add to the file.
   */
  void addSample(int16_t sample);

  /**
   * @brief Add a sample to the WAV file. This is a slower operation.
   *
   * @param sample - A double that will be converted to a 16-bit signed sample.
   */
  void addSample(double sample);

  /**
   * @brief Save the file and close it.
   */
  void done();

private:
  std::ofstream wav_file_{};
  std::streampos data_start_ = 0;
};

} // namespace wavgen

#endif /* WAV_FILE_HPP_ */