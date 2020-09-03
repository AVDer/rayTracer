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

// Schlick Approximation
double_t schlick(double_t cosine, double_t refr_index) {
  auto r0 = (1 - refr_index) / (1 + refr_index);
  r0 = r0 * r0;
  return r0 + (1 - r0) * pow((1 - cosine), 5);
}

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
  Metal(const color_t &albedo, double_t fuzz)
      : albedo_(albedo), fuzz_(std::min(fuzz, 1.)) {}

  std::optional<scatter_t> scatter([[maybe_unused]] const Ray &ray,
                                   const hit_t &hit) const override {
    scatter_t result;
    auto reflected = reflect(unit_vector(ray.direction()), hit.normal);
    result.scattered =
        Ray(hit.point, reflected + fuzz_ * random_in_sphere<double_t>());
    result.attenuation = albedo_;
    if (dot(result.scattered.direction(), hit.normal) > 0) {
      return result;
    } else {
      return std::nullopt;
    }
  }

public:
  color_t albedo_;
  double_t fuzz_;
};

class Dielectric : public Material {
public:
  Dielectric(double_t refr_index) : refr_index_(refr_index) {}

  virtual std::optional<scatter_t> scatter(const Ray &ray,
                                           const hit_t &rec) const override {
    scatter_t result;
    result.attenuation = color_t(1.0, 1.0, 1.0);
    double_t etai_over_etat =
        rec.front_face ? (1.0 / refr_index_) : refr_index_;

    vec3d_t unit_direction = unit_vector(ray.direction());

    double_t cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
    double_t sin_theta = sqrt(1.0 - cos_theta * cos_theta);
    if (etai_over_etat * sin_theta > 1.0) {
      vec3d_t reflected = reflect(unit_direction, rec.normal);
      result.scattered = Ray(rec.point, reflected);
      return result;
    }

    double_t reflect_prob = schlick(cos_theta, etai_over_etat);
    if (rt::random_number<double_t>() < reflect_prob) {
      vec3d_t reflected = reflect(unit_direction, rec.normal);
      result.scattered = Ray(rec.point, reflected);
      return result;
    }

    vec3d_t refracted = refract(unit_direction, rec.normal, etai_over_etat);
    result.scattered = Ray(rec.point, refracted);
    return result;
  }

  double_t refr_index_;
};

#endif // SC_MATERIAL_H
