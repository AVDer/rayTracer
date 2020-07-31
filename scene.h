#ifndef SC_SCENE_H
#define SC_SCENE_H

#include <memory>

#include "circle.h"
#include "objects.h"
#include "ppm_image.h"
#include "ray.h"
#include "vec3.h"

class Scene {

  static constexpr double_t kAspectRatio{16.0 / 9.0};

  // Viewport settings
  static constexpr double_t kViewportHeight{2.0};
  static constexpr double_t kViewportWidth{kViewportHeight * kAspectRatio};
  static constexpr double_t kFocalLength{1.0};

  // Camera

  static constexpr auto kOrigin{point3_t(0, 0, 0)};
  static constexpr auto kHorizontal = vec3d_t(kViewportWidth, 0, 0);
  static constexpr auto kVertical = vec3d_t(0, kViewportHeight, 0);
  static constexpr auto kLowerLeftCorner =
      kOrigin - kHorizontal / 2 - kVertical / 2 - vec3d_t(0, 0, kFocalLength);

public:
  Scene() {
    // std::cerr << kLowerLeftCorner << std::endl;
    objects_.add(std::make_shared<Circle>(point3_t(0, 0, -1), 0.5));
    objects_.add(std::make_shared<Circle>(point3_t(0, -100.5, -1), 100));
  }

  void set_width(size_t width) {
    width_ = width;
    height_ = static_cast<size_t>(width_ / kAspectRatio);
    image_.resize(width_, height_);
  }

  void generate() {
    for (size_t j{0}; j < height_; ++j) {
      for (size_t i{0}; i < width_; ++i) {
        auto u = double(i) / (width_ - 1);
        auto v = double(j) / (height_ - 1);
        Ray r(kOrigin,
              kLowerLeftCorner + u * kHorizontal + v * kVertical - kOrigin);
        color_t pixel_color = ray_color(r);
        image_.set_pixel(i, height_ - j - 1, pixel_color);
      }
    }
    image_.print();
  }

private:
  color_t ray_color(const Ray &r) {
    // std::cerr << r << std::endl;
    auto h = objects_.hit(r, 0, std::numeric_limits<double_t>::infinity());
    if (h.has_value()) {
      auto hit = h.value();
      return 0.5 * (hit.normal + color_t(1, 1, 1));
    }

    vec3d_t unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color_t(1.0, 1.0, 1.0) + t * color_t(0.5, 0.7, 1.0);
  }

  size_t width_;
  size_t height_;

  PpmImage image_;

  Objects objects_;
};

#endif // SC_SCENE_H