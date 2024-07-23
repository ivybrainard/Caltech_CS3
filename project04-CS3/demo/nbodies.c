#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "scene.h"
#include "sdl_wrapper.h"
#include "state.h"
#include "body.h"
#include "vector.h"
#include "forces.h"

const size_t NUM_POINTS = 8;
const double GRAV_CONST = 50;
const double PI = 3.14159265359;
const size_t N = 100;
const int32_t RAD_UPPER = 20;
const int32_t RAD_LOWER = 10;
const int32_t VEL_RANGE = 80;
const int32_t MASS_MULT = 10;

const vector_t ORIGIN = (vector_t){.x = 0, .y = 0};
const vector_t WINDOW = (vector_t){.x = 1000, .y = 500};

struct state {
  scene_t *scene;
};

body_t *create_star() {
  list_t *coordinates = list_init(NUM_POINTS, free);
  double radius = (double) (rand() % RAD_UPPER + RAD_LOWER);
  vector_t start = (vector_t) {.x = rand() % (int) (WINDOW.x/2) + (WINDOW.x/4), 
                                .y = rand() % (int) (WINDOW.y/2) + (WINDOW.y/4)};
  for (size_t i = 0; i < NUM_POINTS; i++) {
    vector_t *point = malloc(sizeof(vector_t));
    double angle = (PI / (NUM_POINTS / 2)) * i;
    if (i % 2 == 0) { 
      point->x = start.x + radius * cos(angle);
      point->y = start.y + radius * sin(angle);
    } else {
      point->x = start.x + radius / 2 * cos(angle);
      point->y = start.y + radius / 2 * sin(angle);
    }
    list_add(coordinates, point);
  }
  double r = (double)rand() / RAND_MAX;
  double g = (double)rand() / RAND_MAX;
  double b = (double)rand() / RAND_MAX;
  rgb_color_t *color = color_init(r, g, b);
  vector_t init_vel = (vector_t) {.x = rand() % VEL_RANGE - (VEL_RANGE/2), 
                                  .y = rand() % VEL_RANGE - (VEL_RANGE/2)};
  body_t *star = body_init(coordinates, radius*MASS_MULT, *color);
  body_set_velocity(star, init_vel);
  return star;
}

state_t *emscripten_init() {
    sdl_init(ORIGIN, WINDOW);
    state_t *state = malloc(sizeof(state_t));
    state->scene = scene_init();
    for (size_t i = 0; i<N; i++) {
      scene_add_body(state->scene, create_star());
    }
    for (size_t j = 0; j<N; j++) {
      for (size_t k = j+1; k<N; k++) {
        create_newtonian_gravity(state->scene, GRAV_CONST, scene_get_body(state->scene, j),
                              scene_get_body(state->scene, k));
      } 
    }
    return state;
}

void emscripten_main(state_t *state) {
  double time = time_since_last_tick();
  scene_tick(state->scene, time);
  sdl_render_scene(state->scene);
}

void emscripten_free(state_t *state) {
  scene_free(state->scene);
  free(state);
}
