#ifndef SC_MISC_H
#define SC_MISC_H

#include <cmath>
#include <random>

namespace rt {

inline double_t deg_to_rad(double_t degrees) { return degrees * M_PIl / 180.0; }

double_t random_double() {
  static std::uniform_real_distribution<double_t> distribution(0.0, 1.0);
  static std::mt19937 generator;
  return distribution(generator);
}

} // namespace rt

#endif