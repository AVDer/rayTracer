#ifndef SC_SCENE_H
#define SC_SCENE_H

#include <atomic>
#include <mutex>
#include <thread>

#include "camera.h"
#include "circle.h"
#include "material.h"
#include "misc.h"
#include "objects.h"
#include "ppm_image.h"

static const double_t kAcne{0.001};
static const uint32_t kSpp{5};
static const uint8_t kThreadsNumber{8};

static size_t lines_counter{0};
std::mutex print_mutex;
std::mutex image_mutex;
PpmImage image;

class Scene {

  static const uint16_t kMaxDepth{10};

public:
  Scene() : camera_(std::make_unique<Camera>(20)) {
    camera_->set_look_from(point3_t(13, 2, 3))
        .set_look_at(point3_t(0, 0, 0))
        .set_view_up(vec3d_t(0, 1, 0))
        .calculate();
    image.set_spp(kSpp);

    auto ground_material = std::make_shared<Lambertian>(color_t(0.5, 0.5, 0.5));
    objects_.add(
        make_shared<Circle>(point3_t(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
      for (int b = -11; b < 11; b++) {
        auto choose_mat = rt::random_number<double_t>();
        point3_t center(a + 0.9 * rt::random_number<double_t>(), 0.2,
                        b + 0.9 * rt::random_number<double_t>());

        if ((center - point3_t(4, 0.2, 0)).length() > 0.9) {
          std::shared_ptr<Material> sphere_material;

          if (choose_mat < 0.8) {
            // diffuse
            auto albedo = color_t::random() * color_t::random();
            sphere_material = std::make_shared<Lambertian>(albedo);
            objects_.add(
                std::make_shared<Circle>(center, 0.2, sphere_material));
          } else if (choose_mat < 0.95) {
            // metal
            auto albedo = color_t::random(0.5, 1);
            auto fuzz = rt::random_number<double_t>(0, 0.5);
            sphere_material = std::make_shared<Metal>(albedo, fuzz);
            objects_.add(
                std::make_shared<Circle>(center, 0.2, sphere_material));
          } else {
            // glass
            sphere_material = std::make_shared<Dielectric>(1.5);
            objects_.add(
                std::make_shared<Circle>(center, 0.2, sphere_material));
          }
        }
      }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    objects_.add(std::make_shared<Circle>(point3_t(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<Lambertian>(color_t(0.4, 0.2, 0.1));
    objects_.add(std::make_shared<Circle>(point3_t(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<Metal>(color_t(0.7, 0.6, 0.5), 0.0);
    objects_.add(std::make_shared<Circle>(point3_t(4, 1, 0), 1.0, material3));
  }

  void set_width(size_t width) {
    width_ = width;
    height_ = static_cast<size_t>(width_ / kAspectRatio);
    image.resize(width_, height_);
  }

  void generate() {
    std::vector<std::unique_ptr<std::thread>> threads;
    for (size_t i{0}; i < kThreadsNumber; ++i) {
      threads.emplace_back(std::make_unique<std::thread>(
          &Scene::generate_thread, this, height_ * i / kThreadsNumber,
          height_ * (i + 1) / kThreadsNumber));
    }
    for (const auto &t : threads) {
      t->join();
    }
    image.print();
  }

  void generate_thread(size_t start, size_t end) {
    for (size_t j{start}; j < end; ++j) {
      {
        std::lock_guard<std::mutex> print_guard(print_mutex);
        fprintf(stderr, "%zu\n", ++lines_counter);
      }
      for (size_t i{0}; i < width_; ++i) {

        color_t color(0, 0, 0);
        for (uint32_t s{0}; s < kSpp; ++s) {
          auto u = (double(i) + rt::random_number<double_t>()) / (width_ - 1);
          auto v = (double(j) + rt::random_number<double_t>()) / (height_ - 1);
          color += ray_color(camera_->get_ray(u, v));
        }
        {
          std::lock_guard<std::mutex> print_guard(image_mutex);
          image.set_pixel(i, height_ - j - 1, color);
        }
      }
    }
  }

private:
  color_t ray_color(const Ray &r, uint16_t depth = kMaxDepth) const {
    if (depth <= 0) {
      return color_t(0, 0, 0);
    }
    // 0,001 to remove shadow acne
    auto h = objects_.hit(r, kAcne, std::numeric_limits<double_t>::infinity());
    if (h.has_value()) {
      auto s = h->material->scatter(r, *h);
      if (s.has_value()) {
        return s->attenuation * ray_color(s->scattered, depth - 1);
      }
      return color_t(0, 0, 0);
    }

    vec3d_t unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color_t(1.0, 1.0, 1.0) + t * color_t(0.5, 0.7, 1.0);
  }

  size_t width_;
  size_t height_;

  std::unique_ptr<Camera> camera_;
  Objects objects_;
};

#endif // SC_SCENE_H
