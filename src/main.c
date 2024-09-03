#include "entities/entity.h"
#include "state/state.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_filesystem.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>
#define FONT_PATH "/home/mukul/mukul/sdl/build/Roboto-Black.ttf"
SDL_Window *init_window() {
  // Create a window
  SDL_Window *window =
      SDL_CreateWindow("SDL2 Red Rectangle", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_SHOWN);
  if (!window) {
    fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
    SDL_Quit();
  }
  return window;
}
SDL_Renderer *init_render(SDL_Window *window) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
  }

  // Create a renderer
  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
  }

  return renderer;
}
struct SelectRect {
  bool is_active;
  SDL_Rect rect;
};
int main(int argc, char *argv[]) {
  // Initialize SDL
  // Main loop flag
  bool running = true;
  SDL_Event event;
  SDL_Window *window = init_window();
  SDL_Renderer *renderer = init_render(window);
  MyState state;
  GameInstance *gi = new_game();
  state.sdl_instance.renderer = renderer;
  state.sdl_instance.window = window;
  add_entity(gi, button_panel_entity());
  add_entity(gi, square_entity_new());
  add_entity(gi, global_select_entity_new());
  add_entity(gi, quad_select_entity_new());
  create_event_cb(gi, ENTITY_EVENT_BUTTON_CLICK);
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
    return 1;
  }
  if (TTF_Init() != 0) {
    fprintf(stderr, "TTF_Init Error: %s\n", TTF_GetError());
    SDL_Quit();
    return 1;
  }
  TTF_Font *font = TTF_OpenFont(FONT_PATH, 20);

  if (font == NULL) {
    printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
    return 1;
  }
  state.sdl_instance.font = font;
  entity_on_init(gi, &state);
  while (running) {
    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT) {
        running = false;
      }
      entity_on_update(gi, &state, &event);
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black
    SDL_RenderClear(renderer);
    entity_on_render(gi, renderer, &state);
    SDL_RenderPresent(renderer);
  }
  entity_on_destroy(gi, &state);
  TTF_CloseFont(font);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();

  return 0;
}
