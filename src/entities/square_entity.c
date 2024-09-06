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
      sq->text_panels = new_array();
      sq->quad_links = new_array();
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
    for (int i = 0; i < s->squares->size; i++) {
      Square *quad = (Square *)get_arr(arr, i)->value;
      SDL_Renderer *renderer = s->sdl_instance.renderer;
      for (int i = 0; i < quad->text_panels->size; i++) {
        TextPanel *panel = (TextPanel *)get_arr(quad->text_panels, i)->value;
        SDL_Texture *texture = panel->sdl_texture;
        SDL_Rect renderQuad = {panel->position.x, panel->position.y,
                               panel->dimension.x, panel->dimension.y};
        SDL_RenderCopy(renderer, texture, NULL, &renderQuad);
      }
      render_quad_links(renderer, quad);
    }
  }
}
void update_quad_panels(Square *active_quad, MyState *state) {
  if (!is_allocated(active_quad->description)) {
    return;
  }
  arr_destroy(active_quad->text_panels);
  active_quad->text_panels = NULL;
  active_quad->text_panels = new_array();
  MyArray *arr_splits = split_text(active_quad->description);
  for (int i = 0; i < arr_splits->size; i++) {
    char *c_ptr = (char *)get_arr(arr_splits, i)->value;
    FontInstance *fi =
        new_font_instance(state, c_ptr, new_rgba(0, 255, 255, 255));
    TextPanel *text_panel = new_text_panel_instance(state, i + 1, fi);
    text_panel->position.x = active_quad->rect->x;
    text_panel->position.y = active_quad->rect->y;
    text_panel->id = 0;
    if (i > 0) {
      TextPanel *last_panel =
          (TextPanel *)get_arr(active_quad->text_panels, i - 1)->value;
      text_panel->id = last_panel->id;
      text_panel->position.x =
          last_panel->sdl_surface->w + last_panel->position.x;
      text_panel->position.y = last_panel->position.y;
      if (text_panel->position.x + text_panel->sdl_surface->w >
          active_quad->rect->x + active_quad->rect->w) {
        text_panel->id = last_panel->id + 1;
        text_panel->position.x = active_quad->rect->x;
        text_panel->position.y = active_quad->rect->y +
                                 (last_panel->sdl_surface->h * text_panel->id);
      }
    }
    push_back(active_quad->text_panels, text_panel, sizeof(TextPanel), "");
  }
}
void entity_event_cb_quad_square_entity(char *k, GameInstance *gi,
                                        MyState *state, void *g) {
  ENTITY_EVENT_TYPES e = event_type_from_char(k);
  if (e == ENTITY_EVENT_DELETE_QUAD && is_allocated(g)) {
    delete_arr(state->squares, *((int *)g)); // bug
  } else if (e == ENTITY_EVENT_ON_KEYSTROKE && is_allocated(g)) {
    Square *active_quad = get_active_quad(state);
    if (!is_allocated(active_quad)) {
      return;
    }
    update_keystroke((char *)((SDL_Keycode *)g), &active_quad->description);
    update_quad_panels(active_quad, state);
  } else if (e == ENTITY_EVENT_QUAD_POSITION_UPDATE && is_allocated(g)) {
    bool is_link = *(int *)g;
    if (is_link) {
      return;
    }
    Square *active_quad = get_active_quad(state);
    if (!is_allocated(active_quad)) {
      return;
    }
    Coordinate *active_coordinate = &state->select_state.active_coordinate;
    active_quad->rect->x = active_coordinate->x;
    active_quad->rect->y = active_coordinate->y;
    update_quad_panels(active_quad, state);
  } else if (e == ENTITY_EVENT_UPDATE_QUAD_DIMENSION && is_allocated(g)) {
    Square *active_quad = get_active_quad(state);
    if (!is_allocated(active_quad)) {
      return;
    }
    int dir = *(int *)g;
    if (state->select_state.is_lcontrol_key_active) {
      active_quad->rect->w += dir * 5;
    }
    if (state->select_state.is_lshift_key_active) {
      active_quad->rect->h += dir * 5;
    }
    update_quad_panels(active_quad, state);
  }
}

void square_entity_init(GameInstance *gi, MyState *state) {
  register_event_cb(gi, ENTITY_EVENT_DELETE_QUAD,
                    entity_event_cb_quad_square_entity);
  register_event_cb(gi, ENTITY_EVENT_ON_KEYSTROKE,
                    entity_event_cb_quad_square_entity);
  register_event_cb(gi, ENTITY_EVENT_QUAD_POSITION_UPDATE,
                    entity_event_cb_quad_square_entity);
  register_event_cb(gi, ENTITY_EVENT_UPDATE_QUAD_DIMENSION,
                    entity_event_cb_quad_square_entity);
  state->squares = NULL;
  state->squares = new_array();
}

void square_entity_destroy(MyState *s) {
  if (s->squares != NULL) {
    for (int i = 0; i < s->squares->size; i++) {
      Square *quad = get_arr(s->squares, i)->value;
      _free(quad->rect);
      _free(quad->description);
      free_text_panel(quad->text_panel);
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