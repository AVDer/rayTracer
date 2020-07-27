#ifndef SC_CIRCLE_H
#define SC_CIRCLE_H

#include "object.h"
#include "vec3.h"

class Circle : public Object {
public:
  Circle(point3_t center, double_t radius) : center_(center), radius_(radius) {}
  bool meet(Ray ray) override {
    auto oc = ray.origin() - center_;
    double_t a = dot(ray.direction(), ray.direction());
    double_t b = 2 * dot(ray.direction(), oc);
    double_t c = dot(oc, oc) - radius_ * radius_;
    double_t d = b * 2 - 4 * a * c;
    return d >= 0;
  }

private:
  point3_t center_;
  double_t radius_;
};

#endif // SC_CIRCLE_H