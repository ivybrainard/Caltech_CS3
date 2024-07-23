#include "sdl_wrapper.h"
#include "color.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <assert.h>
#include <asset_cache.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include <float.h>

#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>

const char WINDOW_TITLE[] = "CS 3";
const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 400;
const double MS_PER_S = 1e3;
const SDL_Color color = {255, 255, 255};
Mix_Chunk *SOUND;

vector_t center;
/**
 * The coordinate difference from the center to the top right corner.
 */
vector_t max_diff;
/**
 * The SDL window where the scene is rendered.
 */
SDL_Window *window;
/**
 * The renderer used to draw the scene.
 */
SDL_Renderer *renderer;
/**
 * The keypress handler, or NULL if none has been configured.
 */
key_handler_t key_handler = NULL;

/**
 * SDL's timestamp when a key was last pressed or released.
 * Used to mesasure how long a key has been held.
 */
click_handler_t click_handler = NULL;

uint32_t key_start_timestamp;
/**
 * The value of clock() when time_since_last_tick() was last called.
 * Initially 0.
 */
clock_t last_clock = 0;

vector_t get_window_center(void) {
  int *width = malloc(sizeof(*width)), *height = malloc(sizeof(*height));
  assert(width != NULL);
  assert(height != NULL);
  SDL_GetWindowSize(window, width, height);
  vector_t dimensions = {.x = *width, .y = *height};
  free(width);
  free(height);
  return vec_multiply(0.5, dimensions);
}

double get_scene_scale(vector_t window_center) {
  // Scale scene so it fits entirely in the window
  double x_scale = window_center.x / max_diff.x,
         y_scale = window_center.y / max_diff.y;
  return x_scale < y_scale ? x_scale : y_scale;
}

vector_t get_window_position(vector_t scene_pos, vector_t window_center) {
  // Scale scene coordinates by the scaling factor
  // and map the center of the scene to the center of the window
  vector_t scene_center_offset = vec_subtract(scene_pos, center);
  double scale = get_scene_scale(window_center);
  vector_t pixel_center_offset = vec_multiply(scale, scene_center_offset);
  vector_t pixel = {.x = round(window_center.x + pixel_center_offset.x),
                    // Flip y axis since positive y is down on the screen
                    .y = round(window_center.y - pixel_center_offset.y)};
  return pixel;
}

/**
 * Converts an SDL key code to a char.
 * 7-bit ASCII characters are just returned
 * and arrow keys are given special character codes.
 */
char get_keycode(SDL_Keycode key) {
  switch (key) {
  case SDLK_LEFT:
    return LEFT_ARROW;
  case SDLK_UP:
    return UP_ARROW;
  case SDLK_RIGHT:
    return RIGHT_ARROW;
  case SDLK_DOWN:
    return DOWN_ARROW;
  case SDLK_SPACE:
    return SPACE_BAR;
  case SDLK_a:
    return A_BUTTON;
  case SDLK_w:
    return W_BUTTON;
  case SDLK_d:
    return D_BUTTON;
  case SDLK_s:
    return S_BUTTON;

  default:
    // Only process 7-bit ASCII characters
    return key == (SDL_Keycode)(char)key ? key : '\0';
  }
}

void render_text(TTF_Font *font, const char *message, vector_t location) {

  SDL_Surface *surfaceMessage = TTF_RenderText_Solid(font, message, color);
  SDL_Texture *Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

  SDL_Rect Message_rect;
  Message_rect.x = location.x;
  Message_rect.y = location.y;
  Message_rect.w = surfaceMessage->w;
  Message_rect.h = surfaceMessage->h;
  SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
  SDL_FreeSurface(surfaceMessage);
  SDL_DestroyTexture(Message);
}

void sdl_init(vector_t min, vector_t max) {
  // Check parameters
  assert(min.x < max.x);
  assert(min.y < max.y);

  center = vec_multiply(0.5, vec_add(min, max));
  max_diff = vec_subtract(max, center);
  SDL_Init(SDL_INIT_EVERYTHING);
  window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT,
                            SDL_WINDOW_RESIZABLE);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
}

bool sdl_is_done(void *state) {
  SDL_Event *event = malloc(sizeof(*event));
  assert(event != NULL);
  while (SDL_PollEvent(event)) {
    switch (event->type) {
    case SDL_QUIT:
      free(event);
      return true;
    case SDL_KEYDOWN:
    case SDL_KEYUP:
      // Skip the keypress if no handler is configured
      // or an unrecognized key was pressed
      if (key_handler == NULL)
        break;
      char key = get_keycode(event->key.keysym.sym);
      if (key == '\0')
        break;

      uint32_t timestamp = event->key.timestamp;
      if (!event->key.repeat) {
        key_start_timestamp = timestamp;
      }
      key_event_type_t type =
          event->type == SDL_KEYDOWN ? KEY_PRESSED : KEY_RELEASED;
      double held_time = (timestamp - key_start_timestamp) / MS_PER_S;
      key_handler(key, type, held_time, state);
      break;
    case SDL_MOUSEBUTTONDOWN:
      asset_cache_handle_buttons(state, event->button.x, event->button.y);
      break;
    case SDL_MOUSEBUTTONUP:
      break;
    }
  }

  free(event);
  return false;
}

void sdl_clear(void) {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);
}

void sdl_draw_polygon(polygon_t *poly, rgb_color_t color) {
  list_t *points = polygon_get_points(poly);
  // Check parameters
  size_t n = list_size(points);
  assert(n >= 3);
  assert(0 <= color.r && color.r <= 1);
  assert(0 <= color.g && color.g <= 1);
  assert(0 <= color.b && color.b <= 1);
  vector_t window_center = get_window_center();
  // Convert each vertex to a point on screen
  int16_t *x_points = malloc(sizeof(*x_points) * n),
          *y_points = malloc(sizeof(*y_points) * n);
  assert(x_points != NULL);
  assert(y_points != NULL);
  for (size_t i = 0; i < n; i++) {
    vector_t *vertex = list_get(points, i);
    vector_t pixel = get_window_position(*vertex, window_center);
    x_points[i] = pixel.x;
    y_points[i] = pixel.y;
  }
  // Draw polygon with the given color
  filledPolygonRGBA(renderer, x_points, y_points, n, color.r * 255,
                    color.g * 255, color.b * 255, 255);
  free(x_points);
  free(y_points);
}

void sdl_show(void) {
  // Draw boundary lines
  vector_t window_center = get_window_center();
  vector_t max = vec_add(center, max_diff),
           min = vec_subtract(center, max_diff);
  vector_t max_pixel = get_window_position(max, window_center),
           min_pixel = get_window_position(min, window_center);
  SDL_Rect *boundary = malloc(sizeof(*boundary));
  assert(boundary != NULL);
  boundary->x = min_pixel.x;
  boundary->y = max_pixel.y;
  boundary->w = max_pixel.x - min_pixel.x;
  boundary->h = min_pixel.y - max_pixel.y;
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderDrawRect(renderer, boundary);
  free(boundary);

  SDL_RenderPresent(renderer);
}

void sdl_render_scene(scene_t *scene, void *aux) {
  sdl_clear();
  size_t body_count = scene_bodies(scene);
  for (size_t i = 0; i < body_count; i++) {
    body_t *body = scene_get_body(scene, i);
    list_t *shape = body_get_shape(body);
    polygon_t *poly = polygon_init(shape, (vector_t){0, 0}, 0, 0, 0, 0);
    sdl_draw_polygon(poly, *body_get_color(body));
    list_free(shape);
  }
  if (aux != NULL) {
    body_t *body = aux;
    sdl_draw_polygon(body_get_polygon(body), *body_get_color(body));
  }
  sdl_show();
}

void sdl_render(SDL_Texture *img, vector_t loc, vector_t size) {
  SDL_Rect *texr = malloc(sizeof(SDL_Rect));
  assert(texr != NULL);
  texr->x = loc.x;
  texr->y = loc.y;
  texr->w = size.x;
  texr->h = size.y;
  SDL_RenderCopy(renderer, img, NULL, texr);
  free(texr);
}

void sdl_on_key(key_handler_t handler) { key_handler = handler; }

double time_since_last_tick(void) {
  clock_t now = clock();
  double difference = last_clock
                          ? (double)(now - last_clock) / CLOCKS_PER_SEC
                          : 0.0; // return 0 the first time this is called
  last_clock = now;
  return difference;
}

SDL_Texture *sdl_display(const char *path) {
  SDL_Texture *img = IMG_LoadTexture(renderer, path);
  return img;
}

TTF_Font *load_font(const char *FONT, int8_t TEXT_SIZE) {
  TTF_Init();
  TTF_Font *font = TTF_OpenFont(FONT, TEXT_SIZE);
  assert(font != NULL);
  return font;
}

SDL_Rect get_bound_box(body_t *body) {
  list_t *bod_points = body_get_shape(body);
  // minimum x and maximum y for top left
  vector_t top_left = {DBL_MAX, -DBL_MAX};
  // maximum x and minimum y for bottom right
  vector_t bottom_right = {-DBL_MAX, DBL_MAX};

  size_t len = list_size(bod_points);

  for (size_t i = 0; i < len; i++) {
    vector_t point = *(vector_t *)list_get(bod_points, i);

    if (point.x < top_left.x) {
      top_left.x = point.x;
    }
    if (point.y > top_left.y) {
      top_left.y = point.y;
    }
    if (point.x > bottom_right.x) {
      bottom_right.x = point.x;
    }
    if (point.y < bottom_right.y) {
      bottom_right.y = point.y;
    }
  }

  vector_t window_center = get_window_center();

  vector_t tl_window = get_window_position(top_left, window_center);
  vector_t br_window = get_window_position(bottom_right, window_center);

  SDL_Rect bound = {.x = tl_window.x,
                    .y = tl_window.y,
                    .w = br_window.x - tl_window.x,
                    .h = br_window.y - tl_window.y};

  return bound;
}

void *load_sound(int channel, const char *sound) {
  int make = Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
  assert(make != -1);
  SOUND = Mix_LoadWAV(sound);
  Mix_PlayChannel(channel, SOUND, 0);
  return SOUND;
}

void free_sound() {
  Mix_FreeChunk(SOUND);
  Mix_CloseAudio();
}