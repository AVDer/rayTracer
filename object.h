#ifndef SC_OBJECT_H
#define SC_OBJECT_H

#include <memory>
#include <optional>

#include "material.h"
#include "ray.h"

class Material;

struct hit_t {
  point3_t point;   // p
  double_t ray_pos; // t
  vec3d_t normal;   // normal

  std::shared_ptr<Material> material;

  // Assume that normal vector is always against the falling ray
  // front face means that it's true
  bool front_face;

  // We always know outward normal (from the sphere senter)
  void set_normal_face(const Ray &ray, const vec3d_t &outward_normal) {
    front_face = dot(ray.direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

class Object {
public:
  virtual std::optional<hit_t> hit(const Ray &ray, double_t tmin,
                                   double_t tmax) = 0;

protected:
  color_t color_;
};

#endif // SC_OBJECT_H