#include "../state/state.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
typedef struct {
  MyArray *entities;
  HashMap *events;
} GameInstance;
GameInstance *new_game();

typedef void (*MouseUpdateFunc)(GameInstance *gi, MyState *state, SDL_Event *event);
typedef void (*register_event_cb_entity)(char *k, GameInstance *gi, MyState *state, void *generic);
typedef void (*on_entity_init)(GameInstance *gi, MyState *s);
typedef void (*on_entity_render)(SDL_Renderer *renderer, MyState *state);
typedef void (*on_entity_destroy)(MyState *state);
typedef struct {
  MouseUpdateFunc on_mouse_update;
  on_entity_init on_entity_init;
  on_entity_render on_entity_render;
  on_entity_destroy on_entity_destroy;
} Entity;
void entity_update(Entity *entity, SDL_Event *event);

// life cycles
void add_entity(GameInstance *gi, Entity *e);
void entity_on_update(GameInstance *gi, void *state, SDL_Event *event);
void entity_on_init(GameInstance *gi, MyState *state);
void entity_on_render(GameInstance *gi, SDL_Renderer *r, MyState *state);
void entity_on_destroy(GameInstance *gi, MyState *state);

// entities
Entity *button_panel_entity();
Entity *global_select_entity_new();
Entity *square_entity_new();
Entity *quad_select_entity_new();
Entity *keyboard_event_listener_entity_new();
Entity *quad_link_entity_new();

// event types
typedef enum {
  ENTITY_EVENT_BUTTON_CLICK,
  ENTITY_EVENT_ON_KEYSTROKE,
  ENTITY_EVENT_DELETE_QUAD,
  ENTITY_EVENT_QUAD_POSITION_UPDATE,
  ENTITY_EVENT_UPDATE_QUAD_DIMENSION,
  ENTITY_EVENT_UNKNOWN
} ENTITY_EVENT_TYPES;

// events
void create_event(GameInstance *gi, ENTITY_EVENT_TYPES event_name);
void register_event_cb(GameInstance *gi, ENTITY_EVENT_TYPES event_name,
                       register_event_cb_entity cb);
void invoke_event_cb(GameInstance *gi, MyState *s, ENTITY_EVENT_TYPES event_name, void *);

const char *char_from_event_type(ENTITY_EVENT_TYPES e);
ENTITY_EVENT_TYPES event_type_from_char(const char *e);