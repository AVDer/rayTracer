#ifndef SC_OBJECT_H
#define SC_OBJECT_H

#include <optional>

#include "ray.h"

struct hit_t {
  point3_t point;
  double_t ray_pos;
  vec3d_t normal;
};

class Object {
public:
  virtual std::optional<hit_t> hit(const Ray &ray, double_t tmin,
                                   double_t tmax) = 0;

protected:
  color_t color_;
};

#endif // SC_OBJECT_H