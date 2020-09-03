#ifndef CS_VEC3_H
#define CS_VEC3_H

#include <iostream>
#include <numbers>

#include "misc.h"

template <typename T> class Vec3 {
public:
  constexpr Vec3() : e_{0, 0, 0} {}
  constexpr Vec3(T e0, T e1, T e2) : e_{e0, e1, e2} {}

  constexpr T x() const { return e_[0]; }
  constexpr T y() const { return e_[1]; }
  constexpr T z() const { return e_[2]; }

  constexpr Vec3 operator-() const { return Vec3(-e_[0], -e_[1], -e_[2]); }
  T operator[](size_t i) const { return e_[i]; }
  T &operator[](size_t i) { return e_[i]; }

  Vec3 &operator+=(const Vec3 &v) {
    e_[0] += v.x();
    e_[1] += v.y();
    e_[2] += v.z();
    return *this;
  }

  template <typename M> Vec3 &operator*=(const M t) {
    e_[0] *= t;
    e_[1] *= t;
    e_[2] *= t;
    return *this;
  }

  template <typename M> Vec3 &operator/=(const M t) { return *this *= 1 / t; }

  constexpr double_t length() const { return std::sqrt(length_squared()); }

  constexpr double_t length_squared() const {
    return e_[0] * e_[0] + e_[1] * e_[1] + e_[2] * e_[2];
  }

  static constexpr Vec3 random() {
    return Vec3(rt::random_number<T>(), rt::random_number<T>(),
                rt::random_number<T>());
  }

  static constexpr Vec3 random(double min, double max) {
    return Vec3(rt::random_number<T>(min, max), rt::random_number<T>(min, max),
                rt::random_number<T>(min, max));
  }

private:
  T e_[3];
};

// Type aliases for vec3
using point3_t = Vec3<double_t>;
using color_t = Vec3<double_t>;
using vec3d_t = Vec3<double_t>;

using point3u = Vec3<uint32_t>;
using coloru = Vec3<uint32_t>;

template <typename T> Vec3<T> random_in_sphere() {
  while (true) {
    auto p = Vec3<T>::random(-1, 1);
    if (p.length_squared() >= 1)
      continue;
    return p;
  }
}

template <typename T> Vec3<T> random_in_hemisphere(const Vec3<T> &normal) {
  Vec3<T> in_unit_sphere = random_in_sphere<T>();
  if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
    return in_unit_sphere;
  else
    return -in_unit_sphere;
}

template <typename T> Vec3<T> random_unit_vector() {
  auto a = rt::random_number<T>(0, 2 * std::numbers::pi);
  auto z = rt::random_number<T>(-1, 1);
  auto r = std::sqrt(static_cast<T>(1) - z * z);
  return Vec3<T>(r * std::cos(a), r * std::sin(a), z);
}

inline std::ostream &operator<<(std::ostream &out, const vec3d_t &v) {
  return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

inline constexpr vec3d_t operator+(const vec3d_t &u, const vec3d_t &v) {
  return vec3d_t(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}

inline constexpr vec3d_t operator-(const vec3d_t &u, const vec3d_t &v) {
  return vec3d_t(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
}

inline constexpr vec3d_t operator*(const vec3d_t &u, const vec3d_t &v) {
  return vec3d_t(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}

inline constexpr vec3d_t operator*(double t, const vec3d_t &v) {
  return vec3d_t(t * v.x(), t * v.y(), t * v.z());
}

inline constexpr vec3d_t operator*(const vec3d_t &v, double t) { return t * v; }

inline constexpr vec3d_t operator/(vec3d_t v, double t) { return (1 / t) * v; }

inline constexpr double dot(const vec3d_t &u, const vec3d_t &v) {
  return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}

inline constexpr vec3d_t cross(const vec3d_t &u, const vec3d_t &v) {
  return vec3d_t(u.y() * v.z() - u.z() * v.y(), u.z() * v.x() - u.x() * v.z(),
                 u.x() * v.y() - u.y() * v.x());
}

inline constexpr vec3d_t unit_vector(vec3d_t v) { return v / v.length(); }

inline constexpr vec3d_t reflect(const vec3d_t &v, const vec3d_t &n) {
  return v - 2 * dot(v, n) * n;
}

inline constexpr vec3d_t refract(const vec3d_t &uv, const vec3d_t &n,
                                 double_t etai_over_etat) {
  auto cos_theta = dot(-uv, n);
  vec3d_t r_out_perp = etai_over_etat * (uv + cos_theta * n);
  vec3d_t r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
  return r_out_perp + r_out_parallel;
}

#endif // CS_VEC3_H
