#ifndef SC_MISC_H
#define SC_MISC_H

#include <numbers>
#include <random>

namespace rt {

inline double_t deg_to_rad(double_t degrees) {
  return degrees * std::numbers::pi / 180.0;
}

template <typename T> T random_number(T min = 0, T max = 1) {
  static std::uniform_real_distribution<T> distribution(min, max);
  static std::mt19937 generator;
  return distribution(generator);
}

} // namespace rt

#endif
