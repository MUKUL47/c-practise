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

  if (event->type == SDL_MOUSEMOTION) {
  } else if (event->button.button == 1) {
    if (event->type == SDL_MOUSEBUTTONDOWN) {
      int mouseX = event->button.x;
      int mouseY = event->button.y;
      for (int i = s->squares->size - 1; i > -1; i--) {
        Square *sq = (Square *)get_arr(s->squares, i)->value;
        SDL_Rect *quad = (SDL_Rect *)sq->rect;
        ButtonPanel bp = s->panel.button_panel[i];
        SDL_Surface *surface = bp.sdl_surface;
        SDL_Texture *texture = bp.sdl_texture;
        if (mouseX >= quad->x && mouseX <= quad->x + quad->w &&
            mouseY >= quad->y && mouseY <= quad->y + quad->h) {
          s->active_selected_quad = sq->id;
          printf("%d\n",sq->id);
          break;
        }
      }
    }
  }
}

void on_entity_render_quad_select_entity(SDL_Renderer *renderer, MyState *s) {}
void entity_on_int_quad_select_entity(GameInstance *gi, MyState *state) {}

void entity_on_destroy_quad_select_entity(MyState *state) {}
Entity *quad_select_entity_new() {
  Entity *e = NULL;
  e = malloc(sizeof(Entity));
  e->on_mouse_update = on_mouse_update_quad_select_entity;
  e->on_entity_init = entity_on_int_quad_select_entity;
  e->on_entity_render = on_entity_render_quad_select_entity;
  e->on_entity_destroy = entity_on_destroy_quad_select_entity;
  return e;
}