#include "object_entity.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

Coordinate *get_shortest_relative_quadray(SDL_Rect *active_quad, MyState *state,
                                          SDL_Rect *dest_quad) {
  assert(is_allocated(state) || is_allocated(dest_quad));
  SDL_Rect *quad = active_quad;
  Coordinate destination;
  if (!is_allocated(state)) {
    destination = new_coords(dest_quad->x, dest_quad->y);
  } else {
    destination = state->select_state.active_coordinate;
  }
  Coordinate coords[] = {
      new_coords(quad->x, quad->y),
      new_coords(quad->x + quad->w, quad->y),
      new_coords(quad->x + quad->w, quad->y + quad->h),
      new_coords(quad->x, quad->y + quad->h),
  };
  int min_v = INT_MAX;
  Coordinate *final_cood = alloc(sizeof(Coordinate));
  *final_cood = new_coords(quad->x, quad->y);
  for (int i = 0; i < 4; i++) {
    int s = abs(destination.x - coords[i].x) + abs(destination.y - coords[i].y);
    if (min_v > s) {
      *final_cood = coords[i];
      min_v = s;
    }
  }
  return final_cood;
}

void render_quad_links(SDL_Renderer *renderer, Square *active_quad) {
  assert(is_allocated(active_quad));
  for (int i = 0; i < active_quad->quad_links->size; i++) {
    QuadLink *link = (QuadLink *)(get_arr(active_quad->quad_links, i)->value);
    SDL_SetRenderDrawColor(renderer, 0, 255, 32, 255);
    int is_dest = is_allocated(link->destination);
    if (is_dest) {
      Coordinate *src =
          get_shortest_relative_quadray(link->source, NULL, link->destination);
      Coordinate *dest =
          get_shortest_relative_quadray(link->destination, NULL, link->source);
      SDL_RenderDrawLine(renderer, src->x, src->y, dest->x, dest->y);
      continue;
    }
    int x = is_dest ? link->destination->x : link->inactive_destination->x;
    int y = is_dest ? link->destination->y : link->inactive_destination->y;
    SDL_RenderDrawLine(renderer, link->source->x, link->source->y, x, y);
  }
}
QuadLink *create_new_quad_link(Square *active_quad, MyState *state) {
  assert(is_allocated(active_quad));
  QuadLink *new_link = alloc(sizeof(QuadLink));
  new_link->source = active_quad->rect;
  new_link->source_coordinate =
      get_shortest_relative_quadray(active_quad->rect, state, NULL);
  new_link->destination = NULL;
  new_link->destination_id = -1;
  Coordinate *c = alloc(sizeof(Coordinate));
  c->x = state->select_state.active_coordinate.x;
  c->y = state->select_state.active_coordinate.y;
  new_link->inactive_destination = c;
  return new_link;
}

void update_quad_link(Square *quad, MyState *state, Square *dest) {
  MyArray *links = quad->quad_links;
  QuadLink *current_link = (QuadLink *)get_arr(links, links->size - 1)->value;
  if (!is_allocated(current_link->destination) && !is_allocated(dest)) {
    current_link->source_coordinate =
        get_shortest_relative_quadray(quad->rect, state, NULL);
    current_link->inactive_destination->x =
        state->select_state.active_coordinate.x;
    current_link->inactive_destination->y =
        state->select_state.active_coordinate.y;
  } else if (is_allocated(dest)) {
    current_link->destination_id = dest->id;
    current_link->destination = dest->rect;
  }
}

void entity_event_cb_quad_link_entity(char *k, GameInstance *gi, MyState *state,
                                      void *g) {
  int is_link_panel = state->panel.active_panel_id != 3;
  ENTITY_EVENT_TYPES e = event_type_from_char(k);
  if (e == ENTITY_EVENT_UNLINK_QUAD && is_allocated(g)) {
    int deleted_quad_id = *((int *)g);
    for (int i = 0; i < state->squares->size; i++) {
      int delete_id = -1;
      Square *quad = (Square *)get_arr(state->squares, i)->value;
      for (int j = 0; j < quad->quad_links->size; j++) {
        QuadLink *link = (QuadLink *)get_arr(quad->quad_links, j)->value;
        if (link->destination_id == deleted_quad_id) {
          delete_id = j;
          break;
        }
      }
      if (delete_id > -1) {
        delete_arr(quad->quad_links, delete_id);
      }
    }
  }
  if (is_link_panel) {
    return;
  }
  if (e == ENTITY_EVENT_QUAD_POSITION_UPDATE && is_allocated(g)) {
    bool is_link = *(int *)g;
    if (!is_link) {
      return;
    }
    Square *active_quad = get_active_quad(state);
    if (!is_allocated(active_quad)) {
      return;
    }
    MyArray *links = active_quad->quad_links;
    if (links->size == 0) {
      push_back(links, (QuadLink *)create_new_quad_link(active_quad, state),
                sizeof(QuadLink), "");
    } else {
      QuadLink *current_link =
          (QuadLink *)get_arr(links, links->size - 1)->value;
      if (!is_allocated(current_link->destination)) {
        update_quad_link(active_quad, state, NULL);
      } else {
        push_back(links, (QuadLink *)create_new_quad_link(active_quad, state),
                  sizeof(QuadLink), "");
      }
    }
  } else if (e == ENTITY_EVENT_MOUSE_KEYUP) {
    Square *prev_quad = get_active_quad(state);
    if (!is_allocated(prev_quad)) {
      return;
    }
    int target_idx = get_quad_on_active_cursor(
        state, state->select_state.active_coordinate.x,
        state->select_state.active_coordinate.y);
    if (target_idx == -1) {
      delete_arr(prev_quad->quad_links, prev_quad->quad_links->size - 1);
      return;
    }
    Square *quad = (Square *)get_arr(state->squares, target_idx)->value;
    for (int j = 0; j < quad->quad_links->size; j++) {
      QuadLink *link = (QuadLink *)get_arr(quad->quad_links, j)->value;
      if (link->destination == prev_quad->rect) {
        delete_arr(prev_quad->quad_links, prev_quad->quad_links->size - 1);
        return;
      }
    }
    for (int j = 0; j < prev_quad->quad_links->size; j++) {
      QuadLink *link = (QuadLink *)get_arr(prev_quad->quad_links, j)->value;
      if (link->destination == quad->rect) {
        delete_arr(prev_quad->quad_links, prev_quad->quad_links->size - 1);
        return;
      }
    }
    update_active_selected_quad(state, quad);
    update_quad_link(prev_quad, state, quad);
  }
}
void init_quad_link_entity(GameInstance *gi, MyState *state) {
  register_event_cb(gi, ENTITY_EVENT_QUAD_POSITION_UPDATE,
                    entity_event_cb_quad_link_entity);
  register_event_cb(gi, ENTITY_EVENT_MOUSE_KEYUP,
                    entity_event_cb_quad_link_entity);
  register_event_cb(gi, ENTITY_EVENT_UNLINK_QUAD,
                    entity_event_cb_quad_link_entity);
}
void render_quad_link_entity(SDL_Renderer *renderer, MyState *state) {}
void update_quad_link_entity(GameInstance *gi, MyState *state,
                             SDL_Event *event) {}
void destroy_quad_link_entity(MyState *state) {}

Entity *quad_link_entity_new() {
  Entity *e = NULL;
  e = alloc(sizeof(Entity));
  e->on_mouse_update = update_quad_link_entity;
  e->on_entity_init = init_quad_link_entity;
  e->on_entity_render = render_quad_link_entity;
  e->on_entity_destroy = destroy_quad_link_entity;
  return e;
}