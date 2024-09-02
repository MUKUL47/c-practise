#pragma once
#include "../ds/ds.h"
#include "../utils/helper.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>

#define BUTTON_PANEL_LENGTH 2
#define BUTTON_PANEL_LABELS                                                    \
  { "Square", "Select" }
typedef enum { NONE, MOUSE_DOWN, MOUSE_UP } MouseMovement;
/////////////

typedef struct {
  Coordinate start_coordinate;
  Coordinate active_coordinate;
  SDL_Rect active_select;
  MouseMovement mouse_movement;
} SelectState;

/////////////

typedef struct {
  Coordinate position;
  Coordinate dimension;
  SDL_Surface *sdl_surface;
  SDL_Texture *sdl_texture;
  char *name;
  int id;
} ButtonPanel;

typedef enum {
  PanelActionSquare,
  PanelActionSelect,
  PanelActionNone
} PanelActiveAction;

typedef struct {
  ButtonPanel button_panel[BUTTON_PANEL_LENGTH];
  int active_panel_id;
  int panel_max_height;
} Panel;

/////////////


typedef struct {
  Coordinate position;
  Coordinate dimension;
  int id;
  SDL_Rect *rect;
} Square;

////

typedef struct {
  SDL_Renderer *renderer;
  SDL_Window *window;
  TTF_Font *font;
} SDLInstance;

typedef struct {
  SelectState select_state;
  Panel panel;
  SDLInstance sdl_instance;
  MyArray *squares;
} MyState;
