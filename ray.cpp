#include "ray.h"

std::ostream &operator<<(std::ostream &out, const Ray &ray) {
  out << "O: " << ray.origin() << "\t D: " << ray.direction() << std::endl;
  return out;
}