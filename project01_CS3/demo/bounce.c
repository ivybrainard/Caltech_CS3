#include "math.h"
#include "polygon.h"
#include "sdl_wrapper.h"
#include "state.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// constants for sizing of star
const double PI = 3.14159265359;
const size_t RADIUS = 100;
const size_t POINTS = 10;
// constants for color of star
const double R = 1;
const double B = 0;
const double G = 1;
// constants for rotation rate and initial velocity
const double initVx = 3;
const double initVy = 3;
const double rotate = 0.01;
// constants for window display
const vector_t ORIGIN = (vector_t){.x = 0, .y = 0};
const vector_t WINDOW = (vector_t){.x = 1000, .y = 500};
const vector_t CENTER = (vector_t){.x = 500, .y = 250};

void checkCol(vec_list_t *coord, vector_t *velocity) {
  for (size_t i = 0; i < vec_list_size(coord); i++) {
    vector_t *vec = vec_list_get(coord, i);
    if (vec->x >= WINDOW.x || vec->x <= 0) {
      velocity->x = (-1) * velocity->x;
      // return true;
    }
    if (vec->y >= WINDOW.y || vec->y <= 0) {
      velocity->y = (-1) * velocity->y;
      // return true;
    }
  }
}

void changePosition(vec_list_t *coordinates, vector_t *velocity) {
  vector_t starCenter = polygon_centroid(coordinates);
  polygon_rotate(coordinates, rotate, starCenter);
  polygon_translate(coordinates, *velocity);
  for (int j = 0; j < POINTS / 2; j++) {
    vector_t *vec = vec_list_get(coordinates, 2 * j);
    double xVal = vec->x;
    double yVal = vec->y;
    double xMax = WINDOW.x;
    double yMax = WINDOW.y;
    if (xVal > xMax || xVal < 0) {
      velocity->x = (-1) * velocity->x;
      polygon_translate(coordinates, *velocity);
      // return;
    }
    if (yVal >= yMax || yVal < 0) {
      velocity->y = (-1) * velocity->y;
      polygon_translate(coordinates, *velocity);
      // return;
    }
  }
}

typedef struct state {
  vec_list_t *coordinates;
  vector_t *velocity;
} state_t;

state_t *emscripten_init() {
  state_t *bouncingStar = malloc(sizeof(state_t));
  vector_t *velocity = malloc(sizeof(vector_t));
  vec_list_t *coordinates = vec_list_init(POINTS);
  velocity->x = initVx;
  velocity->y = initVy;
  bouncingStar->velocity = velocity;
  sdl_init(ORIGIN, WINDOW);
  for (int i = 0; i < POINTS; i++) {
    vector_t *point = malloc(sizeof(vector_t));
    double angle = (PI / (POINTS / 2)) * i;
    if (i % 2 == 0) { // outside points
      point->x = CENTER.x + RADIUS * cos(angle);
      point->y = CENTER.y + RADIUS * sin(angle);
    } else {
      point->x = CENTER.x + RADIUS / 2 * cos(angle);
      point->y = CENTER.y + RADIUS / 2 * sin(angle);
    }
    vec_list_add(coordinates, point);
  }
  bouncingStar->coordinates = coordinates;
  return bouncingStar;
}

void emscripten_main(state_t *state) {
  sdl_clear();
  vec_list_t *coord = state->coordinates;
  vector_t *vel = state->velocity;
  changePosition(coord, vel);
  checkCol(coord, vel);
  sdl_draw_polygon(coord, R, G, B);
  sdl_show();
}

void emscripten_free(state_t *state) {
  free(state->coordinates);
  free(state->velocity);
  free(state);
}