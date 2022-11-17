/**
 * @file wavgenerator.cpp
 * @author Joshua Jerred
 * @brief A simple wav file generator. Akash Murthy's C++ audio programming
 * series was heavily referenced when creating this library.
 * @version 1.0
 * @date 2022-10-02
 * @copyright Copyright (c) 2022
 */

#include <iostream>
#include <cmath>
#include <fstream>
#include <string>

#include "wavgen.h"

WavGen::WavGen(std::string filename) {
    wav_file_.open(filename, std::ios::binary);


    wav_file_ << "RIFF****WAVE"; // RIFF header
    wav_file_ << "fmt "; // format
    writeBytes(16, 4); // size
    writeBytes(1, 2); // compression code
    writeBytes(1, 2); // number of channels
    writeBytes(sample_rate_, 4); // sample rate
    writeBytes(sample_rate_ * bits_per_sample_ / 8, 4 ); // Byte rate
    writeBytes(bits_per_sample_ / 8, 2); // block align
    writeBytes(bits_per_sample_, 2); // bits per sample
    wav_file_ << "data****"; // actual follows this

    data_start_ = wav_file_.tellp(); // Save the position of the start of the
                                     // data chunk
}

WavGen::~WavGen() {
    wav_file_.close();
}

void WavGen::done() {
    data_end_ = wav_file_.tellp(); // Save the position of the end of the data
                                   // chunk

    wav_file_.seekp(data_start_ - 4); // Go to the beginning of the data chunk
    writeBytes(data_end_ - data_start_, 4); // and write the size of the chunk.
    wav_file_.seekp(4, std::ios::beg); // Go to the beginning of the file
    writeBytes(data_end_ - 8, 4); // Write the size of the overall file
    wav_file_.close();
}

void WavGen::addSineWave(int freq, float amp, float duration) {
    float offset = 2 * M_PI * freq / sample_rate_; // The offset of the angle
                                                   // between samples
    
    // These 2 values are persistent through the for Loop
    float frequency = freq;
    float amplitude = amp;
    

    for(int i = 0; i < std::floor(sample_rate_ * duration); i++ ) { // For each sample
        wave_angle_ += offset;
        int sample = static_cast<int> ((amplitude * sin(wave_angle_)) * max_amplitude_);
        writeBytes(sample, 2);
    }
}

void WavGen::writeBytes(int data, int size) {
    wav_file_.write(reinterpret_cast<const char*> (&data), size);
}