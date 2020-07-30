#ifndef SC_CIRCLE_H
#define SC_CIRCLE_H

#include "object.h"
#include "vec3.h"

class Circle : public Object {
public:
  Circle(point3_t center, double_t radius) : center_(center), radius_(radius) {}

  bool meet(const Ray &ray) override {
    solve(ray);
    return k4_ >= 0;
  }

  std::optional<double_t> hit_point(const Ray &ray) override {
    solve(ray);
    if (k4_ < 0)
      return std::nullopt;
    return (-k2_ - std::sqrt(k4_)) / k1_;
  }

  std::optional<vec3d_t> hit_normal(const Ray &ray) override {
    solve(ray);
    if (k4_ < 0)
      return std::nullopt;
    double_t t = (-k2_ - std::sqrt(k4_)) / k1_;
    return unit_vector(ray.at(t) - center_);
  }

private:
  point3_t center_;
  double_t radius_;

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