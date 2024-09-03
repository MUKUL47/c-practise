#include "helper.h"
Coordinate new_coords(int x, int y) {
  Coordinate c = {x, y};
  return c;
}
RGBA new_rgba(int r, int g, int b, int a) {
  RGBA c = {r, g, b, a};
  return c;
}

void *alloc(size_t s) {
  void *m = NULL;
  m = malloc(s);
  assert(m != NULL);
  return m;
}

void _free(void *p) {
  if (p != NULL) {
    free(p);
  }
}