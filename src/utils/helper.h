#include <stdlib.h>
#include <assert.h>
typedef struct {
  int x, y;
} Coordinate;
typedef struct {
  int r, g, b, a;
} RGBA;
Coordinate new_coords(int x, int y);
RGBA new_rgba(int r, int g, int b, int a);
void *alloc(size_t s);
void _free(void *p);
void *alloc_r(void *p, size_t new_size);