
#include "../state/state.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
typedef struct {
  SDL_Texture *textTexture;
  SDL_Surface *textSurface;
} FontInstance;
FontInstance *load_text(SDLInstance *sdl_instance, char *text, RGBA rgba);