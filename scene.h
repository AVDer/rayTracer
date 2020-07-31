#ifndef SC_SCENE_H
#define SC_SCENE_H

#include "camera.h"
#include "circle.h"
#include "misc.h"
#include "objects.h"
#include "ppm_image.h"

class Scene {

  const uint32_t kSpp{10};

public:
  Scene() {
    // std::cerr << kLowerLeftCorner << std::endl;
    image_.set_spp(kSpp);
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
        color_t color(0, 0, 0);
        for (uint32_t s{0}; s < kSpp; ++s) {
          auto u = (double(i) + rt::random_double()) / (width_ - 1);
          auto v = (double(j) + rt::random_double()) / (height_ - 1);
          color += ray_color(camera_.get_ray(u, v));
        }
        image_.set_pixel(i, height_ - j - 1, color);
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

  Camera camera_;
  Objects objects_;
};

#endif // SC_SCENE_H