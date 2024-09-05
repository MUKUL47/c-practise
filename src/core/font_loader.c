
#include "font_loader.h"
#include <assert.h>
FontInstance *load_text(SDLInstance *sdl_instance, char *text, RGBA rgba) {
  SDL_Renderer *renderer = sdl_instance->renderer;
  SDL_Window *window = sdl_instance->window;
  TTF_Font *font = sdl_instance->font;
  SDL_Color textColor = {rgba.r, rgba.g, rgba.b, rgba.a};
  SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
  if (textSurface == NULL) {
    printf("Unable to render text surface! TTF_Error: %s\n", TTF_GetError());
    return NULL;
  }
  SDL_Texture *textTexture =
      SDL_CreateTextureFromSurface(renderer, textSurface);
  if (textTexture == NULL) {
    printf("Unable to create texture from rendered text! SDL_Error: %s\n",
           SDL_GetError());
    return NULL;
  }
  FontInstance *fi = NULL;
  fi = alloc(sizeof(FontInstance));
  fi->textSurface = textSurface;
  fi->textTexture = textTexture;
  return fi;
}