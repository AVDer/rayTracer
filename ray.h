#ifndef SC_RAY_H
#define SC_RAY_H

#include "vec3.h"

class Ray {
public:
  Ray() = default;
  Ray(const point3_t &origin, const vec3d_t &direction)
      : origin_(origin), direction_(direction) {}

  point3_t origin() const { return origin_; }
  vec3d_t direction() const { return direction_; }

  point3_t at(double t) const { return origin_ + t * direction_; }

public:
  point3_t origin_;
  vec3d_t direction_;
};

#endif // SC_RAY_H