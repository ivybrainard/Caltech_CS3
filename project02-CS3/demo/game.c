//#include "state.h"
#include "list.h"
#include "sdl_wrapper.h"
#include "state.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

const int WIDTH = 800;
const int HEIGHT = 600;
const char *IMG_PATH = "assets/cs3_logo.png";
const char *IMG_PATH_2 = "assets/cs2_logo.png";
const char *Font = "assets/Roboto-Regular.ttf";

typedef struct state {
  SDL_Texture *texture;  // picture one
  SDL_Texture *picture2; // added by Ivy
  double time;
} state_t;

state_t *emscripten_init() {
  state_t *state = malloc(sizeof(state_t));
  vector_t min = {0.0, 0.0};
  vector_t max = {WIDTH, HEIGHT};
  sdl_init(min, max);
  state->texture = sdl_display(IMG_PATH);
  state->picture2 = sdl_display(IMG_PATH_2);
  state->time = 0.0; // added by ivy
  return state;
}

void emscripten_main(state_t *state) {

  // ADDED BY IVY
  // this makes the messgae for "clocl: time"
  char *message = malloc(sizeof(char) * 10);
  sprintf(message, "Clock: %d", (int)state->time); // this oen

  sdl_render(state->texture); // print out thr first picture
  render_text(Font, message); // print out the text with timeer
  sdl_show();

  // stwiching the imagine every 5 seconds: this is the timer
  double change = time_since_last_tick();
  while (change < 1.0) {
    change += time_since_last_tick();
  }
  state->time += change;

  if (((int)state->time % 10 < 6) && ((int)state->time % 10 > 0)) {
    sdl_clear();
    sdl_render(state->texture);
    render_text(Font, message);
    sdl_show();
  } else {
    sdl_clear();
    sdl_render(state->picture2);
    render_text(Font, message);
    sdl_show();
  }

  render_text(Font, message);
  sdl_show();

  // ENDED ADDED BY IVY
}

void emscripten_free(state_t *state) {
  SDL_DestroyTexture(state->texture);
  SDL_DestroyTexture(state->picture2);
  free(state);
}
