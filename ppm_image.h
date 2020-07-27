#ifndef SC_PPM_IMAGE_H
#define SC_PPM_IMAGE_H

#include <random>
#include <vector>

#include "vec3.h"

class PpmImage {

  const uint16_t kMaxColor = 255;

public:
  void resize(size_t width, size_t height) {
    height_ = height;
    width_ = width;
    data_.resize(height);
    for (auto &v : data_) {
      v.resize(width);
    }
  }

  void set_pixel(size_t x, size_t y, color_t color) {
    data_[y][x] = color * kMaxColor;
  }

  void random() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distrib(0, kMaxColor);
    for (auto &v : data_) {
      for (auto &e : v) {
        e = color_t(static_cast<double_t>(distrib(gen)),
                    static_cast<double_t>(distrib(gen)),
                    static_cast<double_t>(distrib(gen)));
      }
    }
  }

  void print() {
    std::cout << "P3\n"
              << width_ << ' ' << height_ << '\n'
              << kMaxColor << '\n';
    for (size_t h{0}; h < height_; ++h) {
      for (size_t w{0}; w < width_; ++w) {
        std::cout << static_cast<uint16_t>(data_[h][w].x()) << ' '
                  << static_cast<uint16_t>(data_[h][w].y()) << ' '
                  << static_cast<uint16_t>(data_[h][w].z()) << '\n';
      }
    }
  }

private:
  size_t width_{0};
  size_t height_{0};
  std::vector<std::vector<color_t>> data_;
};

#endif // SC_PPM_IMAGE_H