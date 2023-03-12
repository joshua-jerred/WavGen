/**
 * @file example.cpp
 * @author Joshua Jerred (https://joshuajer.red)
 * @brief An example of how to use the WavGen library.
 * @date 2023-03-11
 * @copyright Copyright (c) 2023
 * @version 1.0
 */

#include "wavgen.h"

int main() {
  WavGen wav("test.wav");
  wav.addSineWave(200, 1, 0.5);
  wav.addSineWave(100, 1, 1);

  // Random Sequence of Sine Waves
  for (int i = 0; i < 20; i++) {
    wav.addSineWave(rand() % 4000, 1, 0.1);
  }

  // Triangle wave
  double sample = 0.0;
  double delta = 0.001;
  int total_samples = 100000;
  for (int i = 0; i < total_samples; i++) {
    wav.addSample(sample);
    sample += delta;
    if (sample > 1.0) {
      delta = -delta;
    } else if (sample < -1.0) {
      delta = -delta;
    }
    if (i % 1000 == 0 && i < total_samples / 2) {
      delta = delta * 1.1;
    } else if (i % 1000 == 0 && i > total_samples / 2) {
      delta = delta * 0.9;
    }
  }

  wav.done();
  return 0;
}