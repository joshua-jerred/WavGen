/**
 * @file constants.hpp
 * @author Joshua Jerred (https://joshuajer.red)
 * @brief WAV file constants.
 * @date 2023-07-22
 * @copyright Copyright (c) 2023
 */

#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_

#include <cstdint>

namespace wavgen {

inline constexpr uint32_t kSampleRate = 48000;
inline constexpr uint32_t kBitsPerSample = 16;
inline constexpr int32_t kMaxAmplitude = (1 << (kBitsPerSample - 1)) - 10;
inline constexpr uint16_t kHeaderSize = 44;

/**
 * @brief The number of samples to smooth the sine wave with when
 * adding a sine wave to the WAV file with the generator.
 */
inline constexpr uint16_t kSineWaveFilterSamples = 150;

inline constexpr uint32_t kSampleRateMs = kSampleRate / 1000;

} // namespace wavgen

#endif /* CONSTANTS_HPP_ */