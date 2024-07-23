#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "scene.h"
#include "sdl_wrapper.h"
#include "state.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdint.h>

const vector_t MIN = {0, 0};
const vector_t MAX = {1000, 500};
const vector_t CENTER = {500, 250};
const char *asan_font = "assets/impact.ttf";
const char *asan_bg_path = "assets/asan_meme.png";
const int8_t num_asan_captions = 3;
const vector_t asan_bg_loc = (vector_t){170, 0};
const vector_t asan_bg_size = (vector_t){660, 440};
const char *asan_captions[] = {"MEMORY LEAKS", "ASAN", "CS3 STUDENTS"};
const vector_t asan_positions[] = {(vector_t){270, 360}, (vector_t){545, 250},
                                   (vector_t){585, 330}};

const char *code_quality_font = "assets/Cascadia.ttf";
const char *code_quality_bg_path = "assets/code_quality_meme.png";
const int8_t num_code_quality_captions = 6;
const vector_t code_quality_bg_loc = (vector_t){250, 0};
const vector_t code_quality_bg_size = (vector_t){500, 500};
const char *code_quality_captions[] = {"if flag == true {",
                                       "    return true;",
                                       "} else {",
                                       "    return false;",
                                       "}",
                                       "return flag;"};
const vector_t code_quality_positions[] = {
    (vector_t){530, 30},  (vector_t){530, 70},  (vector_t){530, 110},
    (vector_t){530, 150}, (vector_t){530, 190}, (vector_t){530, 350}};

const char *malloc_font = "assets/impact.ttf";
const char *malloc_bg_path = "assets/malloc_meme.png";
const int8_t num_malloc_captions = 2;
const vector_t malloc_bg_loc = (vector_t){250, 0};
const vector_t malloc_bg_size = (vector_t){500, 500};
const char *malloc_captions[] = {"FOR ALL MALLOC I SEE",
                                 "A FREE THERE SHOULD BE"};
const vector_t malloc_positions[] = {(vector_t){300, 50}, (vector_t){300, 450}};

const int8_t NUM_MEMES = 3;
const int8_t TEXT_SIZE = 48;
const SDL_Color BLUE = {0, 0, 255};

typedef struct meme {
  SDL_Texture *bg;
  const char **captions;
  const vector_t *locations;
  int8_t num_captions;
  TTF_Font *font;
  int8_t FONT_SIZE;
  vector_t bg_loc;
  vector_t bg_size;
} meme_t;

typedef struct state {
  meme_t memes[NUM_MEMES];
  meme_t *cur_meme;
  size_t meme_index;
  SDL_Renderer *renderer;
} state_t;

state_t *emscripten_init() {
  state_t *state = malloc(sizeof(state_t));
  assert(state != NULL);
  meme_t *meme1 = malloc(sizeof(meme_t));
  assert(meme1 != NULL);
  meme_t *meme2 = malloc(sizeof(meme_t));
  assert(meme2 != NULL);
  meme_t *meme3 = malloc(sizeof(meme_t));
  assert(meme3 != NULL);
  SDL_Window *window = SDL_CreateWindow(
      "memes", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, MAX.x, MAX.y, 0);

  SDL_Renderer *rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  state->renderer = rend;
  TTF_Init();

  meme1->bg = IMG_LoadTexture(rend, asan_bg_path);
  meme1->captions = asan_captions;
  meme1->locations = asan_positions;
  meme1->num_captions = num_asan_captions;
  TTF_Font *font1 = TTF_OpenFont(asan_font, TEXT_SIZE);
  meme1->font = font1;
  meme1->FONT_SIZE = TEXT_SIZE;
  meme1->bg_loc = asan_bg_loc;
  meme1->bg_size = asan_bg_size;

  meme2->bg = IMG_LoadTexture(rend, code_quality_bg_path);
  meme2->captions = code_quality_captions;
  meme2->locations = code_quality_positions;
  meme2->num_captions = num_code_quality_captions;
  TTF_Font *font2 = TTF_OpenFont(code_quality_font, TEXT_SIZE);
  meme2->font = font2;
  meme2->FONT_SIZE = TEXT_SIZE;
  meme2->bg_loc = code_quality_bg_loc;
  meme2->bg_size = code_quality_bg_size;

  meme3->bg = IMG_LoadTexture(rend, malloc_bg_path);
  meme3->captions = malloc_captions;
  meme3->locations = malloc_positions;
  meme3->num_captions = num_malloc_captions;
  TTF_Font *font3 = TTF_OpenFont(malloc_font, TEXT_SIZE);
  meme3->font = font3;
  meme3->FONT_SIZE = TEXT_SIZE;
  meme3->bg_loc = malloc_bg_loc;
  meme3->bg_size = malloc_bg_size;

  state->memes[0] = *meme1;
  state->memes[1] = *meme2;
  state->memes[2] = *meme3;
  state->meme_index = 0;
  state->cur_meme = &state->memes[state->meme_index];
  free(meme1);
  free(meme2);
  free(meme3);
  return state;
}

void check_mouse(state_t *state) {
  SDL_Event event;
  if (SDL_PollEvent(&event)) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
      printf("(%d, %d)\n", event.button.x, event.button.y);
      state->meme_index = (state->meme_index + 1) % NUM_MEMES;
      state->cur_meme = &state->memes[state->meme_index];
    }
  }
}

void display(state_t *state) {

  meme_t *this_meme = state->cur_meme;

  SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);
  SDL_RenderClear(state->renderer);

  SDL_Rect *img_rect = malloc(sizeof(SDL_Rect));
  img_rect->x = this_meme->bg_loc.x;
  img_rect->y = this_meme->bg_loc.y;
  img_rect->w = this_meme->bg_size.x;
  img_rect->h = this_meme->bg_size.y;

  SDL_RenderCopy(state->renderer, this_meme->bg, NULL, img_rect);

  for (size_t i = 0; i < this_meme->num_captions; i++) {
    SDL_Surface *surfaceMessage =
        TTF_RenderText_Solid(this_meme->font, this_meme->captions[i], BLUE);

    SDL_Texture *Message =
        SDL_CreateTextureFromSurface(state->renderer, surfaceMessage);
    SDL_Rect *Message_rect = malloc(sizeof(SDL_Rect));
    Message_rect->x = this_meme->locations[i].x;
    Message_rect->y = this_meme->locations[i].y;
    Message_rect->w = surfaceMessage->w;
    Message_rect->h = surfaceMessage->h;

    SDL_RenderCopy(state->renderer, Message, NULL, Message_rect);

    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);
    free(Message_rect);
  }
  free(img_rect);
}

void emscripten_main(state_t *state) {
  display(state);
  SDL_RenderPresent(state->renderer);
  check_mouse(state);
  SDL_RenderPresent(state->renderer);
}

void emscripten_free(state_t *state) {
  SDL_DestroyRenderer(state->renderer);
  for (size_t i = 0; i < NUM_MEMES; i++) {
    SDL_DestroyTexture(state->memes[i].bg);
    TTF_CloseFont(state->memes[i].font);
  }
  free(state);
  TTF_Quit();
}