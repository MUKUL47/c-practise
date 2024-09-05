#include "object_entity.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <assert.h>
void update_keystroke(char *k, char **d) {
  char *destination = *d;
  bool is_backspace = (int)*k == KEYSTROKE_BACKSPACE;
  if (is_backspace && (destination == NULL ||
                       (destination != NULL && strlen(destination) < 1))) {
    return;
  }
  if (is_backspace) {
    size_t s = strlen(destination);
    destination[s - 1] = '\0';
    destination = realloc(destination, s);
  } else if (destination == NULL) {
    *d = alloc(sizeof(char) * 2);
    (*d)[0] = *k;
    (*d)[1] = '\0';
  } else {
    size_t current_length = strlen(*d);
    *d = realloc(*d, current_length + sizeof(char) * 2);
    (*d)[current_length] = *k;
    (*d)[current_length + 1] = '\0';
  }
}
void on_mouse_update_keyboard_event_listener(GameInstance *gi, MyState *s,
                                             SDL_Event *event) {
  if (event->type == SDL_KEYDOWN) {
    invoke_event_cb(gi, s, ENTITY_EVENT_ON_KEYSTROKE, &event->key.keysym.sym);
  }
}

void on_entity_render_keyboard_event_listener(SDL_Renderer *renderer,
                                              MyState *s) {}

void keyboard_event_listener_entity_init(GameInstance *gi, MyState *state) {}

void keyboard_event_listener_entity_destroy(MyState *s) {}
Entity *keyboard_event_listener_entity_new() {
  Entity *e = NULL;
  e = alloc(sizeof(Entity));
  e->on_mouse_update = on_mouse_update_keyboard_event_listener;
  e->on_entity_init = keyboard_event_listener_entity_init;
  e->on_entity_render = on_entity_render_keyboard_event_listener;
  e->on_entity_destroy = keyboard_event_listener_entity_destroy;
  return e;
}