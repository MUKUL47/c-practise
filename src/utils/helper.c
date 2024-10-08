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

void *alloc_r(void *p, size_t new_size) {
  void *t = NULL;
  t = realloc(p, new_size);
  assert(t != NULL);
  p = t;
  return p;
}

int is_allocated(void *p){
  return p != NULL;
}