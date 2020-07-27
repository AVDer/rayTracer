#ifndef SC_OBJECT_H
#define SC_OBJECT_H

#include "ray.h"

class Object {
public:
  virtual bool meet(Ray ray) = 0;
};

#endif // SC_OBJECT_H