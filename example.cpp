/**
 * @file example.cpp
 * @author Joshua Jerred (https://joshuajer.red)
 * @brief An example of how to use the WavGen library.
 * @date 2023-03-11
 * @copyright Copyright (c) 2023
 * @version 1.0
 */

#include "wav_gen.hpp"

int main() {
  wavgen::Generator wav("test.wav");
  wav.addSineWave(200, 1, 500);
  wav.addSineWave(100, 1, 1000);

  // Random Sequence of Sine Waves
  for (int i = 0; i < 20; i++) {
    wav.addSineWave(rand() % 4000, 1, 100);
  }

  // Triangle wave
  int16_t sample = 0.0;
  int16_t delta = 50;
  int total_samples = 880000;
  for (int i = 0; i < total_samples; i++) {
    wav.addSample(sample);
    sample += delta;
    if (sample > 32000) {
      delta = -delta;
    } else if (sample < -32000) {
      delta = -delta;
    }
    if (i % 10000 == 0 && i < total_samples / 2) {
      delta = delta * 1.1;
    } else if (i % 10000 == 0 && i > total_samples / 2) {
      delta = delta * 0.9;
    }
  }

  wav.done();
  return 0;
}