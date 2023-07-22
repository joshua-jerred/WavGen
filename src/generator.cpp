/**
 * @file wavgenerator.cpp
 * @author Joshua Jerred
 * @brief A library for generating wav files.
 * @version 1.0
 * @date 2022-10-02
 * @copyright Copyright (c) 2022
 */

#include "wavgen.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>

#define SINE_FILTER_SAMPLES                                                    \
  150 // Look at the sine wave in Audacity, you'll see the smoothing on sine
      // waves

void WavGen::addSineWave(int freq, float amp, float duration) {
  float offset = 2 * M_PI * freq / sample_rate_; // The offset of the angle
                                                 // between samples

  // These 2 values are persistent through the for Loop
  float amplitude = amp;

  int total_samples = std::floor(sample_rate_ * duration);
  float filter = 0.0f; // Filter to reduce the amplitude of the wav in the
                       // first and last SINE_FILTER_SAMPLES samples
  float filter_step =
      1.0f / SINE_FILTER_SAMPLES; // The amount to increase the filter by each
                                  // sample to get a smooth transition

  for (int i = 0; i < total_samples; i++) { // For each sample
    wave_angle_ += offset;
    int sample = static_cast<int>((filter * amplitude * sin(wave_angle_)) *
                                  max_amplitude_);
    writeBytes(sample, 2);

    if (wave_angle_ > 2 * M_PI) {
      wave_angle_ -= 2 * M_PI;
    }

    if (i < SINE_FILTER_SAMPLES) { // Adjust the filter
      filter += filter_step;
    } else if (i > total_samples - SINE_FILTER_SAMPLES) {
      filter -= filter_step;
    } else {
      filter = 1.0f;
    }
  }
}

void WavGen::addSineWaveSamples(int freq, float amplitude, int total_samples) {
  float offset = 2 * M_PI * freq / sample_rate_; // The offset of the angle
                                                 // between samples
  if (total_samples < 0) {
    return;
  }

  for (int i = 0; i < total_samples; i++) { // For each sample
    wave_angle_ += offset;
    int sample =
        static_cast<int>((amplitude * sin(wave_angle_)) * max_amplitude_);
    writeBytes(sample, 2);

    if (wave_angle_ > 2 * M_PI) {
      wave_angle_ -= 2 * M_PI;
    }
  }
}