# WavGen

A simple C++ library for generating wav files.

Basic Features:
- Add sine waves with a given frequency, amplitude, and duration
- Add individual samples

## Usage
```c++
// Create a new wav file object
WavGen wav(std::string filename); 

// Add a sine wave to the wav file
wav.addSineWave(int frequency_hz, double amplitude, double duration_seconds);

// Add a single sample to the wav file (0.0 - 1.0) (default sample rate is 44100)
wav.addSample(double sample);

// Finish writing the wav file
wav.done();
```


Example:
```c++
#include "WavGen.h"

int main() {
  // Create a new wav file
  WavGen wav("test.wav");

  // Add Two Since waves
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
```

Copyright (c) 2023, Joshua Jerred