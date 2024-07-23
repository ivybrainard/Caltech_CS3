#include "sdl_wrapper.h"
#include "state.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 800
#define HEIGHT 600

state_t *emscripten_init() {
  state_t *state = malloc(sizeof(state_t));
  SDL_Window *win =
      SDL_CreateWindow("Image Loading", 100, 100, WIDTH, HEIGHT, 0);
  state->renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
  state->texture = IMG_LoadTexture(state->renderer, "assets/cs3_logo.png");
  return state;
}

void emscripten_main(state_t *state) {
  /** Write your game code here. */

  int *w = malloc(sizeof(int));
  int *h = malloc(sizeof(int));

  SDL_QueryTexture(state->texture, NULL, NULL, w, h);
  SDL_Rect *texr = malloc(sizeof(SDL_Rect));

  int actual_w = WIDTH / 2;
  int actual_h = (*h) * actual_w / (*w);
  int x = (WIDTH - actual_w) / 2;
  int y = (HEIGHT - actual_h) / 2;

  texr->x = x;
  texr->y = y;
  texr->w = actual_w;
  texr->h = actual_h;

  SDL_RenderClear(state->renderer);
  // SDL_Rect destRect = { x, y, actual_w, actual_h };
  SDL_RenderCopy(state->renderer, state->texture, NULL, texr);
  SDL_RenderPresent(state->renderer);
}

void emscripten_free(state_t *state) {
  free(state->texture);
  free(state->renderer);
  free(state);
}
