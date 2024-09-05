#include "object_entity.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <assert.h>
#define PANEL_OVERFLOW 10
void on_mouse_update_button_panel(GameInstance *gi, MyState *s,
                                  SDL_Event *event) {
  if (event->type == SDL_MOUSEBUTTONDOWN) {
    if (event->button.button == SDL_BUTTON_LEFT) {
      int mouseX = event->button.x;
      int mouseY = event->button.y;
      for (int i = 0; i < BUTTON_PANEL_LENGTH; i++) {
        TextPanel bp = s->panel.button_panel[i];
        SDL_Surface *surface = bp.sdl_surface;
        SDL_Texture *texture = bp.sdl_texture;
        if (mouseX >= bp.position.x &&
            mouseX <= bp.position.x + bp.dimension.x &&
            mouseY >= bp.position.y &&
            mouseY <= bp.position.y + bp.dimension.y) {
          s->panel.active_panel_id =
              s->panel.active_panel_id == bp.id ? 0 : bp.id;
          invoke_event_cb(gi, s, ENTITY_EVENT_BUTTON_CLICK, NULL);
          break;
        }
      }
    }
  }
}

void on_entity_render_button_panel(SDL_Renderer *renderer, MyState *s) {
  for (int i = 0; i < BUTTON_PANEL_LENGTH; i++) {
    TextPanel bp = s->panel.button_panel[i];
    SDL_Texture *texture = bp.sdl_texture;
    SDL_Rect renderQuad = {bp.position.x, bp.position.y, bp.dimension.x,
                           bp.dimension.y};
    if (s->panel.active_panel_id == i + 1) {
      SDL_SetTextureColorMod(texture, 255, 0, 0);
    } else {
      SDL_SetTextureColorMod(texture, 255, 255, 255);
    }
    SDL_RenderCopy(renderer, texture, NULL, &renderQuad);
  }
}

void free_text_panel(TextPanel *tp) {
  // _free(tp->sdl_texture);
  // _free(tp->sdl_surface);
  // _free(tp->name);
}

FontInstance *new_font_instance(MyState *s, char *text_content, RGBA rgba) {
  FontInstance *fi = load_text(&s->sdl_instance, text_content, rgba);
  if (fi == NULL) {
    return NULL;
  }
  return fi;
}

TextPanel *new_text_panel_instance(MyState *s, int id, FontInstance *fi) {
  if (fi == NULL) {
    return NULL;
  }
  TextPanel *bp = alloc(sizeof(TextPanel));
  bp->sdl_surface = fi->textSurface;
  bp->sdl_texture = fi->textTexture;
  bp->dimension.x = fi->textSurface->w;
  bp->dimension.y = fi->textSurface->h;
  bp->position.x = 0;
  bp->position.y = 0;
  bp->id = id;
  return bp;
}

TextPanel *create_new_panel(GameInstance *gi, MyState *s, char *name,
                            int index) {
  RGBA rgba = {255, 255, 255, 255};
  FontInstance *fi = new_font_instance(s, name, rgba);
  TextPanel *bp = new_text_panel_instance(s, index + 1, fi);
  if (bp == NULL) {
    return NULL;
  }
  if (index > 0) {
    TextPanel last_panel = s->panel.button_panel[index - 1];
    bp->position.x =
        last_panel.position.x + last_panel.dimension.x + PANEL_OVERFLOW;
    if (last_panel.position.x + last_panel.dimension.x + PANEL_OVERFLOW +
            fi->textSurface->w + bp->position.x >
        WIN_WIDTH) {

      bp->position.y =
          last_panel.position.y + fi->textSurface->h + PANEL_OVERFLOW;
      bp->position.x = 0;
    }
  }
  return bp;
}

void button_panel_init(GameInstance *gi, MyState *s) {
  s->panel.active_panel_id = 0;
  s->panel.panel_max_height = 0;
  char *labels[] = BUTTON_PANEL_LABELS;
  for (int i = 0; i < BUTTON_PANEL_LENGTH; i++) {
    TextPanel *sq = create_new_panel(gi, s, labels[i], i);
    if (sq != NULL) {
      s->panel.button_panel[i] = *sq;
      int c = sq->position.y + sq->dimension.y;
      s->panel.panel_max_height =
          s->panel.panel_max_height < c ? c : s->panel.panel_max_height;
    }
  }
}
void button_panel_destroy(MyState *s) {
  for (int i = 0; i < BUTTON_PANEL_LENGTH; i++) {
    SDL_DestroyTexture(s->panel.button_panel[i].sdl_texture);
  }
}
Entity *button_panel_entity() {
  Entity *e = NULL;
  e = alloc(sizeof(Entity));
  e->on_mouse_update = on_mouse_update_button_panel;
  e->on_entity_init = button_panel_init;
  e->on_entity_render = on_entity_render_button_panel;
  e->on_entity_destroy = button_panel_destroy;
  return e;
}