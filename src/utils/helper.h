typedef struct {
  int x, y;
} Coordinate;
typedef struct {
  int r, g, b, a;
} RGBA;
Coordinate new_coords(int x, int y);
RGBA new_rgba(int r, int g, int b, int a);