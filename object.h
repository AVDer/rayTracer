#ifndef SC_OBJECT_H
#define SC_OBJECT_H

#include <optional>

#include "ray.h"

class Object {
public:
  virtual bool meet(const Ray &ray) = 0;
  virtual std::optional<double_t> hit_point(const Ray &ray) = 0;
  virtual std::optional<vec3d_t> hit_normal(const Ray &ray) = 0;

protected:
  color_t color_;
};

#endif // SC_OBJECT_H