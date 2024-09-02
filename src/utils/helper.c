#include "helper.h"
Coordinate new_coords(int x, int y) {
  Coordinate c = {x, y};
  return c;
}
RGBA new_rgba(int r, int g, int b, int a) {
  RGBA c = {r, g, b, a};
  return c;
}