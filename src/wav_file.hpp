/**
 * @file wav_file.hpp
 * @author Joshua Jerred (https://joshuajer.red)
 * @brief A simple WAV file structure (16-bit, single channel)
 * @date 2023-07-21
 * @copyright Copyright (c) 2023
 */

#ifndef WAV_FILE_HPP_
#define WAV_FILE_HPP_

#include <fstream>
#include <vector>

#include "constants.hpp"

namespace wavgen {

/**
 * @brief The base class for WAV files.
 *
 */
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
   * @brief Get the number of samples in the WAV file.
   * @return uint32_t - The number of samples in the WAV file.
   */
  virtual uint32_t getNumSamples() = 0;

  /**
   * @brief Get the duration of the WAV file in milliseconds.
   * @return uint32_t - The duration in milliseconds.
   */
  virtual uint32_t getDuration() = 0;

  /**
   * @brief Get the size of the WAV file in bytes.
   * @return uint32_t - The size of the WAV file in bytes.
   */
  virtual uint32_t getFileSize() = 0;
};

/**
 * @brief A class to write WAV files.
 */
class Writer : public WavFile {
public:
  /**
   * @brief Open a WAV file for writing.
   *
   * @param output_file_path - The name of the file to write to.
   */
  Writer(std::string output_file_path);

  /**
   * @brief Deconstructor for the WAV file writer. This will call done().
   */
  ~Writer();

  uint32_t getNumSamples() override;
  uint32_t getDuration() override;
  uint32_t getFileSize() override;

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
};

class Generator : public Writer {
public:
  Generator(std::string output_file_path) : Writer(output_file_path) {
  }
  ~Generator() = default;

  /**
   * @brief Add a sine wave to the WAV file with a given frequency, amplitude,
   * and duration.
   *
   * @param frequency - The frequency of the sine wave in Hz.
   * @param amplitude - The amplitude of the sine wave (0.0 - 1.0)
   * @param duration - The duration of the sine wave in milliseconds.
   */
  void addSineWave(uint16_t frequency, double amplitude, uint16_t duration_ms);

  /**
   * @brief Add a sine wave to the WAV file for a given number of samples.
   *
   * @param frequency - The frequency of the sine wave in Hz.
   * @param amplitude - The amplitude of the sine wave (0.0 - 1.0)
   * @param samples - The number of samples to add to the WAV file.
   */
  void addSineWaveSamples(uint16_t frequency, double amplitude,
                          uint32_t samples);

private:
  /**
   * @brief The angle of the sine wave, persistent to get a continuous wave.
   */
  double wave_angle_ = 0.0f;
};

/**
 * @brief A class to read WAV files.
 */
class Reader : public WavFile {
public:
  /**
   * @brief Open a WAV file for reading.
   *
   * @param input_file_path - The name of the file to read from.
   */
  Reader(std::string input_file_path);

  /**
   * @brief Deconstructor for the WAV file reader.
   */
  ~Reader() = default;

  uint32_t getNumSamples() override;
  uint32_t getDuration() override;
  uint32_t getFileSize() override;

  void getAllSamples(std::vector<int16_t> &samples);

private:
  std::ifstream wav_file_{};
};
} // namespace wavgen

#endif /* WAV_FILE_HPP_ */