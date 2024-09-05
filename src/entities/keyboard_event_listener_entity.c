#include "object_entity.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <assert.h>
#include <string.h>
void arr_append_space(char **current_str, MyArray *arr, int k) {
  char *current = *current_str;
  current = alloc_r(current, k + sizeof(char));
  current[k] = '\0';
  int c_size = strlen(current);
  if (c_size > 0) {
    push_back(arr, (char *)current, c_size + 1, "");
    char *space = " ";
    push_back(arr, space, strlen(space) + 1, "");
  }
}

MyArray *split_text(char *text) { //, bool(cb_split)(char *c)) {
  MyArray *split_arr = new_array();
  char *current = NULL;
  int k = 0;
  size_t current_size = strlen(text);
  for (int i = 0; i < current_size; i++) {
    char c = text[i];
    int is_space = (int)c == 32;
    if (is_space) {
      if (current == NULL) {
        char *space = " ";
        push_back(split_arr, space, strlen(space) + 1, "");
        continue;
      }
      arr_append_space(&current, split_arr, k);
      current = NULL;
      k = 0;
    } else {
      current = !is_allocated(current)
                    ? alloc(sizeof(char))
                    : alloc_r(current, strlen(current) + sizeof(char));
      current[k++] = c;
    }
  }
  if (current != NULL) {
    current = alloc_r(current, k + sizeof(char));
    current[k] = '\0';
    int c_size = strlen(current);
    if (c_size > 0) {
      push_back(split_arr, (char *)current, c_size + 1, "");
      current = NULL;
    }
  }
  return split_arr;
}
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