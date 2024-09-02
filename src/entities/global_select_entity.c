#include "entity.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
void on_mouse_update_select(GameInstance *gi, MyState *s, SDL_Event *event) {
  // if (s->panel.active_panel_id > 0) {
  //   return;
  // }
  SelectState *select_state = &s->select_state;
  Coordinate *active_coordinate = &s->select_state.active_coordinate;
  MouseMovement *mouse_movement = &s->select_state.mouse_movement;
  Coordinate *start_coordinate = &s->select_state.start_coordinate;

  if (event->type == SDL_MOUSEMOTION) {
    SDL_GetMouseState(&active_coordinate->x, &active_coordinate->y);
  } else if (event->button.button == 1) {
    if (event->type == SDL_MOUSEBUTTONDOWN) {
      *mouse_movement = MOUSE_DOWN;
      *start_coordinate =
          new_coords(active_coordinate->x, active_coordinate->y);
    } else if (event->type == SDL_MOUSEBUTTONUP) {

      *mouse_movement = MOUSE_UP;
    }
  }
}

void on_entity_render_select(SDL_Renderer *renderer, MyState *s) {
  SelectState *select_state = &s->select_state;
  Coordinate *active_coordinate = &s->select_state.active_coordinate;
  MouseMovement *mouse_movement = &s->select_state.mouse_movement;
  Coordinate *start_coordinate = &s->select_state.start_coordinate;
  SDL_Rect *active_select = &s->select_state.active_select;
  if (*mouse_movement == MOUSE_DOWN) {
    SDL_SetRenderDrawColor(renderer, 230, 0, 200, 255);
    active_select->x = start_coordinate->x;
    active_select->y = start_coordinate->y;
    active_select->w = active_coordinate->x - start_coordinate->x;
    active_select->h = active_coordinate->y - start_coordinate->y;
    SDL_RenderDrawRect(renderer, active_select);
  }
}

void mock_init(GameInstance *gi, MyState *state) {}
Entity *global_select_entity_new() {
  Entity *e = NULL;
  e = malloc(sizeof(Entity));
  e->on_mouse_update = on_mouse_update_select;
  e->on_entity_init = mock_init;
  e->on_entity_render = on_entity_render_select;
  return e;
}