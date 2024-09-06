#include "entity.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <assert.h>

bool is_cursor_on_quad(int mouseX, int mouseY, SDL_Rect *quad) {
  return mouseX > quad->x && mouseX < (quad->x + quad->w) && mouseY > quad->y &&
         mouseY < (quad->y + quad->h);
}
int get_quad_on_active_cursor(MyState *s, int x, int y) {
  for (int i = s->squares->size - 1; i > -1; i--) {
    Square *sq = (Square *)get_arr(s->squares, i)->value;
    SDL_Rect *quad = (SDL_Rect *)sq->rect;
    if (is_cursor_on_quad(x, y, quad)) {
      return i;
    }
  }
  return -1;
}
void update_active_selected_quad(MyState *s, Square *sq) {
  s->active_selected_quad = is_allocated(sq) ? sq->id : 0;
}
void on_mouse_update_quad_select_entity(GameInstance *gi, MyState *s,
                                        SDL_Event *event) {
  if (s->panel.active_panel_id != 2 && s->panel.active_panel_id != 3) {
    return;
  }
  Coordinate *active_coordinate = &s->select_state.active_coordinate;
  MouseMovement *mouse_movement = &s->select_state.mouse_movement;

  if (event->type == SDL_MOUSEBUTTONDOWN) {
    int mouseX = event->button.x;
    int mouseY = event->button.y;
    int delete_idx = -1;
    int i = get_quad_on_active_cursor(s, mouseX, mouseY);
    if (i > -1) {
      Square *sq = (Square *)get_arr(s->squares, i)->value;
      if (event->button.button == 3) {
        delete_idx = i;
      } else if (event->button.button == 1) {
        update_active_selected_quad(s, sq);
      }
    }
    if (delete_idx > -1) {
      invoke_event_cb(gi, s, ENTITY_EVENT_DELETE_QUAD, &delete_idx);
    }
  }
  int is_link = s->panel.active_panel_id == 3;
  if (event->type == SDL_MOUSEMOTION && event->button.button == 1 &&
      s->active_selected_quad > 0) {
    MouseMovement *mouse_movement = &s->select_state.mouse_movement;
    if (*mouse_movement == MOUSE_DOWN) {
      invoke_event_cb(gi, s, ENTITY_EVENT_QUAD_POSITION_UPDATE, &is_link);
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