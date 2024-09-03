#include "entity.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <assert.h>
void on_mouse_update_square(GameInstance *gi, MyState *s, SDL_Event *event) {
  if (s->panel.active_panel_id != 1) {
    return;
  }

  SelectState *select_state = &s->select_state;
  Coordinate *active_coordinate = &s->select_state.active_coordinate;
  MouseMovement *mouse_movement = &s->select_state.mouse_movement;
  Coordinate *start_coordinate = &s->select_state.start_coordinate;

  if (event->button.button == 1) {
    if (event->type == SDL_MOUSEBUTTONUP) {
      Square *sq = alloc(sizeof(Square));
      sq->rect = alloc(sizeof(SDL_Rect));
      sq->id = s->squares->size + 1;
      if (active_coordinate->y < s->panel.panel_max_height) {
        return;
      }
      sq->rect->x = start_coordinate->x;
      sq->rect->y = start_coordinate->y;
      sq->rect->w = active_coordinate->x - start_coordinate->x;
      sq->rect->h = active_coordinate->y - start_coordinate->y;
      push_back(s->squares, sq, sizeof(Square), "");
    }
  }
}

void on_entity_render_square(SDL_Renderer *renderer, MyState *s) {
  MyArray *arr = s->squares;
  for (int i = 0; i < s->squares->size; i++) {
    Square *quad = (Square *)get_arr(arr, i)->value;
    if (s->active_selected_quad == quad->id) {
      SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    } else {
      SDL_SetRenderDrawColor(renderer, 0, 0, 200, 255);
    }
    SDL_RenderDrawRect(renderer, quad->rect);
  }
}
void square_entity_init(GameInstance *gi, MyState *state) {
  state->squares = NULL;
  state->squares = new_array();
}

void square_entity_destroy(MyState *s) {
  if (s->squares != NULL) {
    for (int i = 0; i < s->squares->size; i++) {
      Square *quad = get_arr(s->squares, i)->value;
      _free(quad->rect);
    }
    arr_destroy(s->squares);
  }
}
Entity *square_entity_new() {
  Entity *e = NULL;
  e = malloc(sizeof(Entity));
  e->on_mouse_update = on_mouse_update_square;
  e->on_entity_init = square_entity_init;
  e->on_entity_render = on_entity_render_square;
  e->on_entity_destroy = square_entity_destroy;
  return e;
}