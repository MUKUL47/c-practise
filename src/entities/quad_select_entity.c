#include "entity.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <assert.h>
void on_mouse_update_quad_select_entity(GameInstance *gi, MyState *s,
                                        SDL_Event *event) {
  if (s->panel.active_panel_id != 2) {
    return;
  }
  Coordinate *active_coordinate = &s->select_state.active_coordinate;
  MouseMovement *mouse_movement = &s->select_state.mouse_movement;

  if (event->type == SDL_MOUSEBUTTONDOWN) {
    int mouseX = event->button.x;
    int mouseY = event->button.y;
    int delete_idx = -1;
    for (int i = s->squares->size - 1; i > -1; i--) {
      Square *sq = (Square *)get_arr(s->squares, i)->value;
      SDL_Rect *quad = (SDL_Rect *)sq->rect;
      TextPanel bp = s->panel.button_panel[i];
      SDL_Surface *surface = bp.sdl_surface;
      SDL_Texture *texture = bp.sdl_texture;
      if (mouseX >= quad->x && mouseX <= quad->x + quad->w &&
          mouseY >= quad->y && mouseY <= quad->y + quad->h) {
        if (event->button.button == 3) {
          delete_idx = i;
        } else if (event->button.button == 1) {
          s->active_selected_quad = sq->id;
        }
        break;
      }
    }
    if (delete_idx > -1) {
      invoke_event_cb(gi, s, ENTITY_EVENT_DELETE_QUAD, &delete_idx);
    }
  }
  if (event->type == SDL_MOUSEMOTION && event->button.button == 1 &&
      s->active_selected_quad > 0) {
    MouseMovement *mouse_movement = &s->select_state.mouse_movement;
    if (*mouse_movement == MOUSE_DOWN) {
      Square *sq =
          (Square *)get_arr(s->squares, s->active_selected_quad - 1)->value;
      Coordinate *active_coordinate = &s->select_state.active_coordinate;
      sq->rect->x = active_coordinate->x;
      sq->rect->y = active_coordinate->y;
      sq->text_panel->position.x = sq->rect->x;
      sq->text_panel->position.y = sq->rect->y;
    }
  }
}

void on_entity_render_quad_select_entity(SDL_Renderer *renderer, MyState *s) {}

void entity_event_cb_quad_select_entity(char *k, GameInstance *gi,
                                        MyState *state, void *g) {
  ENTITY_EVENT_TYPES e = event_type_from_char(k);
  if (e == ENTITY_EVENT_BUTTON_CLICK) {
    if (state->panel.active_panel_id != 2) {
      state->active_selected_quad = 0;
    }
  }
}

void entity_on_int_quad_select_entity(GameInstance *gi, MyState *state) {
  state->active_selected_quad = 0;
  register_event_cb(gi, ENTITY_EVENT_BUTTON_CLICK,
                    entity_event_cb_quad_select_entity);
}
void entity_on_destroy_quad_select_entity(MyState *state) {}
Entity *quad_select_entity_new() {
  Entity *e = NULL;
  e = alloc(sizeof(Entity));
  e->on_mouse_update = on_mouse_update_quad_select_entity;
  e->on_entity_init = entity_on_int_quad_select_entity;
  e->on_entity_render = on_entity_render_quad_select_entity;
  e->on_entity_destroy = entity_on_destroy_quad_select_entity;
  return e;
}