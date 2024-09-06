#include "./entity.h"
#include <SDL2/SDL_render.h>
#include <assert.h>
#include <string.h>

const char *char_from_event_type(ENTITY_EVENT_TYPES e) {
  switch (e) {
  case ENTITY_EVENT_BUTTON_CLICK:
    return "ENTITY_EVENT_BUTTON_CLICK";
  case ENTITY_EVENT_DELETE_QUAD:
    return "ENTITY_EVENT_DELETE_QUAD";
  case ENTITY_EVENT_ON_KEYSTROKE:
    return "ENTITY_EVENT_ON_KEYSTROKE";
  case ENTITY_EVENT_QUAD_POSITION_UPDATE:
    return "ENTITY_EVENT_QUAD_POSITION_UPDATE";
  case ENTITY_EVENT_UPDATE_QUAD_DIMENSION:
    return "ENTITY_EVENT_UPDATE_QUAD_DIMENSION";
  default:
    return "ENTITY_EVENT_UNKNOWN";
  }
}

ENTITY_EVENT_TYPES event_type_from_char(const char *e) {
  if (strcmp(e, "ENTITY_EVENT_BUTTON_CLICK") == 0) {
    return ENTITY_EVENT_BUTTON_CLICK;
  } else if (strcmp(e, "ENTITY_EVENT_DELETE_QUAD") == 0) {
    return ENTITY_EVENT_DELETE_QUAD;
  } else if (strcmp(e, "ENTITY_EVENT_ON_KEYSTROKE") == 0) {
    return ENTITY_EVENT_ON_KEYSTROKE;
  } else if (strcmp(e, "ENTITY_EVENT_QUAD_POSITION_UPDATE") == 0) {
    return ENTITY_EVENT_QUAD_POSITION_UPDATE;
  } else if (strcmp(e, "ENTITY_EVENT_UPDATE_QUAD_DIMENSION") == 0) {
    return ENTITY_EVENT_UPDATE_QUAD_DIMENSION;
  }
  return ENTITY_EVENT_UNKNOWN;
}

GameInstance *new_game() {
  GameInstance *gi = NULL;
  gi = alloc(sizeof(GameInstance));
  gi->entities = new_array();
  gi->events = hm_new(NULL);
  return gi;
}

void add_entity(GameInstance *gi, Entity *e) {
  push_back(gi->entities, e, sizeof(Entity), "");
}

void entity_on_update(GameInstance *gi, void *state, SDL_Event *event) {
  for (int i = 0; i < gi->entities->size; i++) {
    Entity *e = (Entity *)(get_arr(gi->entities, i)->value);
    if (e->on_mouse_update != NULL) {
      e->on_mouse_update(gi, state, event);
    }
  }
}

void entity_on_init(GameInstance *gi, MyState *state) {
  for (int i = 0; i < gi->entities->size; i++) {
    Entity *e = (Entity *)(get_arr(gi->entities, i)->value);
    if (e->on_entity_init != NULL) {
      e->on_entity_init(gi, state);
    }
  }
}

void entity_on_render(GameInstance *gi, SDL_Renderer *r, MyState *state) {
  for (int i = 0; i < gi->entities->size; i++) {
    Entity *e = (Entity *)(get_arr(gi->entities, i)->value);
    if (e->on_entity_render != NULL) {
      e->on_entity_render(r, state);
    }
  }
}

void entity_on_destroy(GameInstance *gi, MyState *state) {
  for (int i = 0; i < gi->entities->size; i++) {
    Entity *e = (Entity *)(get_arr(gi->entities, i)->value);
    if (e->on_entity_destroy != NULL) {
      e->on_entity_destroy(state);
    }
  }
  arr_destroy(gi->entities);
  hm_destroy(gi->events);
  gi->entities = NULL;
  gi->events = NULL;
}

void create_event(GameInstance *gi, ENTITY_EVENT_TYPES e) {
  char *event_name = strdup(char_from_event_type(e));
  hm_insert(gi->events, event_name, new_array(), event_name);
}

void register_event_cb(GameInstance *gi, ENTITY_EVENT_TYPES e,
                       register_event_cb_entity cb) {
  char *event_name = strdup(char_from_event_type(e));
  if (!hm_has_key(gi->events, event_name)) {
    return;
  }
  MyArray *arr = (MyArray *)hm_get(gi->events, event_name)->value;
  push_back(arr, &cb, sizeof(register_event_cb_entity), event_name);
  hm_insert(gi->events, event_name, arr, event_name);
}

void invoke_event_cb(GameInstance *gi, MyState *s, ENTITY_EVENT_TYPES e,
                     void *generic) {
  char *event_name = strdup(char_from_event_type(e));
  if (!hm_has_key(gi->events, event_name)) {
    return;
  }
  MyArray *arr = (MyArray *)hm_get(gi->events, event_name)->value;
  for (int i = 0; i < arr->size; i++) {
    register_event_cb_entity *cb =
        (register_event_cb_entity *)(get_arr(arr, i)->value);
    (*cb)(event_name, gi, s, generic);
  }
}