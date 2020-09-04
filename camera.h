#ifndef SC_CAMERA_H
#define SC_CAMERA_H

#include "misc.h"
#include "ray.h"

static constexpr double_t kAspectRatio{16.0 / 9.0};

// Viewport settings
static constexpr double_t kFocalLength{1.0};

class Camera {
public:
  Camera(double_t vfov) {
    auto theta = rt::deg_to_rad(vfov);
    auto h = tan(theta / 2);
    viewport_height_ = 2.0 * h;
    viewport_width_ = kAspectRatio * viewport_height_;
  }

  Camera &set_look_from(point3_t look_from) {
    look_from_ = look_from;
    return *this;
  }

  Camera &set_look_at(point3_t look_at) {
    look_at_ = look_at;
    return *this;
  }

  Camera &set_view_up(vec3d_t view_up) {
    view_up_ = view_up;
    return *this;
  }

  void calculate() {
    auto w = unit_vector(look_from_ - look_at_);
    auto u = unit_vector(cross(view_up_, w));
    auto v = cross(w, u);

    origin_ = look_from_;
    horizontal_ = viewport_width_ * u;
    vertical_ = viewport_height_ * v;
    lower_left_corner_ = origin_ - horizontal_ / 2 - vertical_ / 2 - w;
  }

  Ray get_ray(double_t s, double_t t) const {
    return Ray(origin_,
               lower_left_corner_ + s * horizontal_ + t * vertical_ - origin_);
  }

private:
  point3_t look_from_;
  point3_t look_at_;
  vec3d_t view_up_;

  double_t viewport_width_;
  double_t viewport_height_;

  point3_t origin_;
  vec3d_t horizontal_;
  vec3d_t vertical_;
  point3_t lower_left_corner_;
};

#endif // SC_CAMERA_H