#include <iostream>

#include "ppm_image.h"

int main() {
  PpmImage image;
  image.resize(768, 432);
  image.random();
  image.print();
}