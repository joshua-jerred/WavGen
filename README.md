[![WavGen Tests (C++ CI)](https://github.com/joshua-jerred/WavGen/actions/workflows/tests.yaml/badge.svg)](https://github.com/joshua-jerred/WavGen/actions/workflows/tests.yaml)

# WavGen

A simple C++ library for generating wav files.

Allows you to read and write wav files along with a few extra features for adding
sine waves and raw samples to a wav file.

Build and link with CMake.

Detailed doxygen documentation can be found in the header files.

## Usage
```c++
#include "wav_gen.hpp"

// Basic Write
wavgen::Writer writer(std::string output_path);
writer.addSample(double sample);
writer.addSample(int16_t sample);
writer.done();

// Basic Read
wavgen::Reader reader(std::string input_path);
std::vector<int16_t> samples;
reader.getAllSamples(samples);

// Generator, publicly inherits from Writer
wavgen::Generator gen(std::string output_path); 
gen.addSineWave(uint16_t frequency, double amplitude, uint16_t duration_ms);
gen.addSineWaveSamples(uint16_t frequency, double amplitude,
                          uint32_t samples);
gen.done();

// Common Methods:
uint32_t getSampleRate() const;
uint32_t getBitsPerSample() const;
uint32_t getNumSamples();
virtual uint32_t getDuration();
virtual uint32_t getFileSize();
```
