#ifndef SC_SCENE_H
#define SC_SCENE_H

#include "camera.h"
#include "circle.h"
#include "material.h"
#include "misc.h"
#include "objects.h"
#include "ppm_image.h"

static const double_t kAcne{0.001};

class Scene {

  static const uint32_t kSpp{1000};
  static const uint16_t kMaxDepth{10};

public:
  Scene() {
    // std::cerr << kLowerLeftCorner << std::endl;
    image_.set_spp(kSpp);

    // Ground
    objects_.add(std::make_shared<Circle>(
        point3_t(0, -100.5, -1), 100,
        std::make_shared<Lambertian>(color_t(0.8, 0.8, 0.0))));

    // Left
    objects_.add(std::make_shared<Circle>(point3_t(-1.0, 0.0, -1.0), 0.5,
                                          std::make_shared<Dielectric>(1.5)));

    // Center
    objects_.add(std::make_shared<Circle>(
        point3_t(0.0, 0.0, -1.0), 0.5,
        std::make_shared<Lambertian>(color_t(0.1, 0.2, 0.5))));

    // Right
    objects_.add(std::make_shared<Circle>(
        point3_t(1.0, 0.0, -1.0), 0.5,
        std::make_shared<Metal>(color_t(0.8, 0.6, 0.2), 0.0)));
  }

  void set_width(size_t width) {
    width_ = width;
    height_ = static_cast<size_t>(width_ / kAspectRatio);
    image_.resize(width_, height_);
  }

  void generate() {
    uint32_t progress{0};
    for (size_t j{0}; j < height_; ++j) {
      for (size_t i{0}; i < width_; ++i) {
        uint32_t new_progress{static_cast<uint32_t>(j * 100 / height_)};
        if (new_progress != progress) {
          progress = new_progress;
          std::cerr << progress << "%\n";
        }
        // std::cerr << "y: " << j << "\tx: " << i << '\n';
        color_t color(0, 0, 0);
        for (uint32_t s{0}; s < kSpp; ++s) {
          auto u = (double(i) + rt::random_number<double_t>()) / (width_ - 1);
          auto v = (double(j) + rt::random_number<double_t>()) / (height_ - 1);
          color += ray_color(camera_.get_ray(u, v));
        }
        image_.set_pixel(i, height_ - j - 1, color);
      }
    }
    image_.print();
  }

private:
  color_t ray_color(const Ray &r, uint16_t depth = kMaxDepth) {
    // std::cerr << r << std::endl;
    if (depth <= 0) {
      return color_t(0, 0, 0);
    }
    // 0,001 to remove shadow acne
    auto h = objects_.hit(r, kAcne, std::numeric_limits<double_t>::infinity());
    if (h.has_value()) {
      // return 0.5 * (hit.normal + color_t(1, 1, 1));

      // point3_t target = hit.point + hit.normal +
      // random_in_sphere<double_t>();

      // point3_t target = hit.point + hit.normal +
      // random_unit_vector<double_t>();

      // point3_t target = hit.point +
      // random_in_hemisphere<double_t>(hit.normal);

      // return 0.5 * ray_color(Ray(hit.point, target - hit.point), depth - 1);

      auto s = h->material->scatter(r, *h);
      if (s.has_value()) {
        return s->attenuation * ray_color(s->scattered, depth - 1);
      }
      return color_t(0, 0, 0);
    }

    vec3d_t unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color_t(1.0, 1.0, 1.0) + t * color_t(0.5, 0.7, 1.0);

    // return color_t(1, 1, 1);
  }

  size_t width_;
  size_t height_;

  PpmImage image_;

  Camera camera_;
  Objects objects_;
};

#endif // SC_SCENE_H
