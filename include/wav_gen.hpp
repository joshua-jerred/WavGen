/**
 * @file wav_gen.hpp
 * @author Joshua Jerred (https://joshuajer.red)
 * @brief A simple WAV file structure (16-bit, single channel)
 * @date 2023-07-21
 * @copyright Copyright (c) 2023
 */

#ifndef WAV_FILE_HPP_
#define WAV_FILE_HPP_

#include <cstdint>
#include <fstream>
#include <vector>

namespace wavgen {

/**
 * @brief The sample rate of the WAV file.
 */
inline constexpr uint32_t SAMPLE_RATE = 48000;
inline constexpr uint32_t SAMPLE_RESOLUTION = 16;
inline constexpr int32_t MAX_SAMPLE_AMPLITUDE =
    (1 << (SAMPLE_RESOLUTION - 1)) - 10;
inline constexpr uint16_t HEADER_SIZE = 44;

/**
 * @brief The number of samples to smooth the sine wave with when
 * adding a sine wave to the WAV file with the generator.
 */
inline constexpr uint16_t SINE_WAVE_SAMPLES_TO_FILTER = 150;

/**
 * @brief The number of samples per millisecond.
 */
inline constexpr uint32_t SAMPLE_RATE_MS = SAMPLE_RATE / 1000;

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
    return SAMPLE_RATE;
  }

  /**
   * @brief Get the number of bits per sample.
   * @return uint32_t - The number of bits per sample.
   */
  uint32_t getBitsPerSample() const {
    return SAMPLE_RESOLUTION;
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
   * @param sample - A 16-bit signed sample to add to the file.
   */
  void addSample(int16_t sample);

  /**
   * @brief Add a sample to the WAV file. This is a slower operation.
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