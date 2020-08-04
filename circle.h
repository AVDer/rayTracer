#ifndef SC_CIRCLE_H
#define SC_CIRCLE_H

#include "material.h"
#include "object.h"
#include "vec3.h"

class Circle : public Object {
public:
  Circle(point3_t center, double_t radius, std::shared_ptr<Material> material)
      : center_(center), radius_(radius), material_(material) {}

  std::optional<hit_t> hit(const Ray &ray, double_t tmin,
                           double_t tmax) override {
    solve(ray);
    if (k4_ >= 0) {
      hit_t result;
      result.ray_pos = (-k2_ - std::sqrt(k4_)) / k1_;
      if (result.ray_pos >= tmin && result.ray_pos <= tmax) {
        result.point = ray.at(result.ray_pos);
        result.set_normal_face(ray, (result.point - center_) / radius_);
        result.material = material_;
        return result;
      }
      result.ray_pos = (-k2_ + std::sqrt(k4_)) / k1_;
      if (result.ray_pos >= tmin && result.ray_pos <= tmax) {
        result.point = ray.at(result.ray_pos);
        result.set_normal_face(ray, (result.point - center_) / radius_);
        result.material = material_;
        return result;
      }
    }

    return std::nullopt;
  }

private:
  point3_t center_;
  double_t radius_;
  std::shared_ptr<Material> material_;

  double_t k1_; // a
  double_t k2_; // b / 2
  double_t k3_; // c
  double_t k4_; // D

  void solve(const Ray &ray) {
    auto oc = ray.origin() - center_;
    k1_ = ray.direction().length_squared();
    k2_ = dot(ray.direction(), oc);
    k3_ = oc.length_squared() - radius_ * radius_;
    k4_ = k2_ * k2_ - k1_ * k3_;
  }
};

#endif // SC_CIRCLE_H