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
      SDL_Rect *active_select = malloc(sizeof(SDL_Rect));
      active_select->x = start_coordinate->x;
      active_select->y = start_coordinate->y;
      active_select->w = active_coordinate->x - start_coordinate->x;
      active_select->h = active_coordinate->y - start_coordinate->y;
      push_back(s->squares, active_select, sizeof(SDL_Rect), "");
    }
  }
}

void on_entity_render_square(SDL_Renderer *renderer, MyState *s) {
  for (int i = 0; i < s->squares->size; i++) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 200, 255);
    SDL_RenderDrawRect(renderer, (SDL_Rect *)get_arr(s->squares, i)->value);
  }
}
void square_entity_init(GameInstance *gi, MyState *state) {
  state->squares = NULL;
  state->squares = new_array();
  assert(state->squares != NULL);
}
Entity *square_entity_new() {
  Entity *e = NULL;
  e = malloc(sizeof(Entity));
  e->on_mouse_update = on_mouse_update_square;
  e->on_entity_init = square_entity_init;
  e->on_entity_render = on_entity_render_square;
  e->on_entity_destroy = NULL;
  return e;
}