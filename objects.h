#ifndef SC_OBJECTS_H
#define SC_OBJECTS_H

#include <memory>
#include <vector>

#include "object.h"

class Objects : public Object {

public:
  Objects() = default;

  void add(std::shared_ptr<Object> object) { objects_.push_back(object); }

  std::optional<hit_t> hit(const Ray &ray, double_t tmin,
                           double_t tmax) override {
    hit_t result;
    result.ray_pos = std::numeric_limits<double_t>::infinity();
    bool found{false};
    for (const auto &o : objects_) {
      auto h = o->hit(ray, tmin, tmax);
      if (h.has_value()) {
        auto H = h.value();
        if (H.ray_pos < result.ray_pos) {
          result = H;
          found = true;
        }
      }
    }
    if (found) {
      return result;
    }
    return std::nullopt;
  }

private:
  std::vector<std::shared_ptr<Object>> objects_;
};

#endif