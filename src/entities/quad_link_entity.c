#include "object_entity.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

void render_quad_links(SDL_Renderer *renderer, Square *active_quad) {
  assert(is_allocated(active_quad));
  for (int i = 0; i < active_quad->quad_links->size; i++) {
    QuadLink *link = (QuadLink *)(get_arr(active_quad->quad_links, i)->value);
    SDL_SetRenderDrawColor(renderer, 0, 255, 32, 255);
    SDL_RenderDrawLine(
        renderer, link->source_coordinate->x, link->source_coordinate->y,
        link->inactive_destination->x, link->inactive_destination->y);
  }
}

Coordinate* get_shortest_relative_quadray(Square *active_quad, MyState *state) {
  SDL_Rect *quad = active_quad->rect;
  Coordinate destination = state->select_state.active_coordinate;
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

QuadLink *create_new_quad_link(Square *active_quad, MyState *state) {
  assert(is_allocated(active_quad));
  QuadLink *new_link = alloc(sizeof(QuadLink));
  new_link->source = active_quad->rect;
  new_link->source_coordinate = get_shortest_relative_quadray(active_quad, state);
  new_link->destination = NULL;
  Coordinate *c = alloc(sizeof(Coordinate));
  c->x = state->select_state.active_coordinate.x;
  c->y = state->select_state.active_coordinate.y;
  new_link->inactive_destination = c;
  return new_link;
}

void entity_event_cb_quad_link_entity(char *k, GameInstance *gi, MyState *state,
                                      void *g) {
  ENTITY_EVENT_TYPES e = event_type_from_char(k);
  if (e == ENTITY_EVENT_QUAD_POSITION_UPDATE && is_allocated(g)) {
    bool is_link = *(int *)g;
    if (!is_link) {
      return;
    }
    printf("%d.%d\n",state->active_selected_quad,state->previous_active_selected_quad);
    Square *active_quad = get_active_quad(state);
    if (!is_allocated(active_quad)) {
      return;
    }
    MyArray *links = active_quad->quad_links;
    if (links->size == 0) {
      QuadLink *new_link = create_new_quad_link(active_quad, state);
      push_back(links, new_link, sizeof(QuadLink), "");
    } else {
      QuadLink *current_link =
          (QuadLink *)get_arr(links, links->size - 1)->value;
      if (!is_allocated(current_link->destination)) {
        current_link->source = active_quad->rect;
        current_link->source_coordinate = get_shortest_relative_quadray(active_quad, state);
        current_link->inactive_destination->x =
            state->select_state.active_coordinate.x;
        current_link->inactive_destination->y =
            state->select_state.active_coordinate.y;
      } else {
        // QuadLink *new_link = create_new_quad_link(active_quad, state);
        // push_back(links, (QuadLink *)new_link, sizeof(QuadLink), "");
      }
    }
  }
}
void init_quad_link_entity(GameInstance *gi, MyState *state) {
  register_event_cb(gi, ENTITY_EVENT_QUAD_POSITION_UPDATE,
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