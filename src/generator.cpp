/**
 * @file wavgenerator.cpp
 * @author Joshua Jerred
 * @brief A library for generating wav files.
 * @version 1.0
 * @date 2022-10-02
 * @copyright Copyright (c) 2022
 */

#include <cmath>

#include "constants.hpp"
#include "wav_gen.hpp"

inline constexpr double pi2() {
  return std::atan(1) * 4 * 2;
}

namespace wavgen {

void Generator::addSineWave(uint16_t frequency, double amplitude,
                            uint16_t duration_ms) {

  /**
   * @brief The delta angle between samples.
   */
  const double d_wave = pi2() * frequency / kSampleRate;

  /**
   * @brief The delta of the filter.
   */
  const double d_filter = 1.0f / kSineWaveFilterSamples;
  const uint32_t total_samples = kSampleRateMs * duration_ms;

  /**
   * @brief Filter to reduce the amplitude of the wave in the first and last
   * kSineWaveFilterSamples samples.
   */
  double filter = 0.0f;

  for (uint32_t i = 0; i < total_samples; i++) { // For each sample
    wave_angle_ += d_wave;
    int16_t sample = static_cast<int16_t>(
        (filter * amplitude * sin(wave_angle_)) * kMaxAmplitude);
    addSample(sample);

    if (wave_angle_ > pi2()) {
      wave_angle_ -= pi2();
    }

    // Adjust the filter
    if (i < kSineWaveFilterSamples) {
      filter += d_filter;
    } else if (i > total_samples - kSineWaveFilterSamples) {
      filter -= d_filter;
    } else {
      filter = 1.0f;
    }
  }
}

void Generator::addSineWaveSamples(uint16_t frequency, double amplitude,
                                   uint32_t samples) {
  float offset = pi2() * frequency / kSampleRate; // The offset of the angle
                                                  // between samples

  for (uint32_t i = 0; i < samples; i++) {        // For each sample
    wave_angle_ += offset;
    int16_t sample =
        static_cast<int16_t>((amplitude * sin(wave_angle_)) * kMaxAmplitude);
    addSample(sample);

    if (wave_angle_ > pi2()) {
      wave_angle_ -= pi2();
    }
  }
}

} // namespace wavgen