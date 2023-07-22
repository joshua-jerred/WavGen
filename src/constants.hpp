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
#include <string>

namespace wavgen {

inline constexpr uint32_t kSampleRate = 480000;
inline constexpr uint32_t kBitsPerSample = 16;
inline constexpr int32_t kMaxAmplitude = (1 << (kBitsPerSample - 1)) - 1;

} // namespace wavgen

#endif /* CONSTANTS_HPP_ */