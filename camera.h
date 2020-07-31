#ifndef SC_CAMERA_H
#define SC_CAMERA_H

#include "ray.h"

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

class Camera {
public:
  Ray get_ray(double_t x, double_t y) const {
    return Ray(kOrigin,
               kLowerLeftCorner + x * kHorizontal + y * kVertical - kOrigin);
  }
};

#endif