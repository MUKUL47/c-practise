#include "object_entity.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <assert.h>
#include <string.h>

Square *get_active_quad(MyState *s) {
  MyArray *arr = s->squares;
  for (int i = 0; i < s->squares->size; i++) {
    Square *quad = (Square *)get_arr(arr, i)->value;
    if (s->active_selected_quad == quad->id) {
      return quad;
    }
  }
  return NULL;
}

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
      sq->description = NULL;
      sq->text_panel = new_text_panel_instance(
          s, 1, new_font_instance(s, " ", new_rgba(0, 255, 0, 255)));
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
    SDL_Texture *texture = quad->text_panel->sdl_texture;
    SDL_Rect renderQuad = {
        quad->text_panel->position.x, quad->text_panel->position.y,
        quad->text_panel->dimension.x, quad->text_panel->dimension.y};
    SDL_RenderCopy(renderer, texture, NULL, &renderQuad);
  }
}
void entity_event_cb_quad_square_entity(char *k, GameInstance *gi,
                                        MyState *state, void *g) {
  ENTITY_EVENT_TYPES e = event_type_from_char(k);
  if (e == ENTITY_EVENT_DELETE_QUAD && g != NULL) {
    delete_arr(state->squares, *((int *)g));
  } else if (e == ENTITY_EVENT_ON_KEYSTROKE && g != NULL) {
    Square *active_quad = get_active_quad(state);
    if (!is_allocated(active_quad)) {
      return;
    }
    update_keystroke((char *)((SDL_Keycode *)g), &active_quad->description);
    active_quad->text_panel = new_text_panel_instance(
        state, 1,
        new_font_instance(state, active_quad->description,
                          new_rgba(0, 255, 255, 255)));
    active_quad->text_panel->position.x = active_quad->rect->x;
    active_quad->text_panel->position.y = active_quad->rect->y;
  }
}

void square_entity_init(GameInstance *gi, MyState *state) {
  register_event_cb(gi, ENTITY_EVENT_DELETE_QUAD,
                    entity_event_cb_quad_square_entity);
  register_event_cb(gi, ENTITY_EVENT_ON_KEYSTROKE,
                    entity_event_cb_quad_square_entity);
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
  e = alloc(sizeof(Entity));
  e->on_mouse_update = on_mouse_update_square;
  e->on_entity_init = square_entity_init;
  e->on_entity_render = on_entity_render_square;
  e->on_entity_destroy = square_entity_destroy;
  return e;
}