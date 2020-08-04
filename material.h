#ifndef SC_MATERIAL_H
#define SC_MATERIAL_H

#include <optional>

#include "object.h"
#include "ray.h"
#include "vec3.h"

struct hit_t;

struct scatter_t {
  color_t attenuation;
  Ray scattered;
};

class Material {
public:
  virtual std::optional<scatter_t> scatter(const Ray &ray,
                                           const hit_t &hit) const = 0;
};

class Lambertian : public Material {
public:
  Lambertian(const color_t &albedo) : albedo_(albedo) {}

  std::optional<scatter_t> scatter([[maybe_unused]] const Ray &ray,
                                   const hit_t &hit) const override {
    scatter_t result;
    result.scattered =
        Ray(hit.point, hit.normal + random_unit_vector<double_t>());
    result.attenuation = albedo_;
    return result;
  }

public:
  color_t albedo_;
};

class Metal : public Material {
public:
  Metal(const color_t &albedo) : albedo_(albedo) {}

  std::optional<scatter_t> scatter([[maybe_unused]] const Ray &ray,
                                   const hit_t &hit) const override {
    scatter_t result;
    auto reflected = reflect(unit_vector(ray.direction()), hit.normal);
    result.scattered = Ray(hit.point, reflected);
    result.attenuation = albedo_;
    if (dot(result.scattered.direction(), hit.normal) > 0) {
      return result;
    } else {
      return std::nullopt;
    }
  }

public:
  color_t albedo_;
};

#endif
