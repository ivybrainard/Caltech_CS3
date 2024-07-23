#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "scene.h"
#include "sdl_wrapper.h"
#include "state.h"

const double WEDGE_ANGLE = M_PI / 3; // angle of pacman's mouth
const double INCREMENT_ANGLE = 0.1;  // angle between points on pacman shape
const double PAC_RADIUS = 60;
const double BALL_RADIUS = 10; // radius of balls (pacman's food)
const int CIRC_NPOINTS = 20;   // points to draw circle

const int ACCELERATION = 100;
const int START_VEL = 100;

const int PACMAN_MASS = 100;
const rgb_color_t PACMAN_COLOR = (rgb_color_t){1, .75, .79};
const rgb_color_t FOOD_COLOR = (rgb_color_t){.3, .3, 1};
const vector_t CENTER = (vector_t){500, 250};

list_t *get_pacman(vector_t center);
void on_key(char key, key_event_type_t type, double held_time,
            state_t *state); // not sure about this one

body_t *make_circle(vector_t center, double radius, double mass,
                    rgb_color_t color);

// the window this is needed
const vector_t ORIGIN = (vector_t){.x = 0, .y = 0};
const vector_t WINDOW = (vector_t){.x = 1000, .y = 500};

/*
NOTE: Add any other fields in state you need for your demo
*/
struct state {
  scene_t *scene;
  body_t *pacman;
  double time;
};

state_t *emscripten_init() {

  state_t *game = malloc(sizeof(state_t));
  vector_t center = (vector_t){CENTER.x, CENTER.y};
  game->time = 0;
  sdl_init(ORIGIN, WINDOW);
  game->scene = scene_init();
  game->pacman = body_init(get_pacman(center), PACMAN_MASS, PACMAN_COLOR);
  scene_add_body(game->scene, game->pacman);
  sdl_on_key((void *)on_key);

  // priting the pellets
  for (size_t i = 0; i < 18; i++) {
    int randomY = (rand() % 500 + 1);
    int randomX = (rand() % 1000 + 1);
    vector_t random_center = {randomX, randomY};
    scene_add_body(game->scene, make_circle(random_center, BALL_RADIUS,
                                            PACMAN_MASS, FOOD_COLOR));
  }
  return game;
}

/** Make a circle-shaped body object.
 *
 * @param center a vector representing the center of the body.
 * @param radius the radius of the circle
 * @param mass the mass of the body
 * @param color the color of the circle
 * @return pointer to the circle-shaped body
 */
body_t *make_circle(vector_t center, double radius, double mass,
                    rgb_color_t color) {
  list_t *c = list_init(CIRC_NPOINTS, free);

  for (size_t i = 0; i < CIRC_NPOINTS; i++) {
    double angle = 2 * M_PI * i / CIRC_NPOINTS;

    vector_t *v = malloc(sizeof(*v));
    assert(v);

    vector_t unit = {cos(angle), sin(angle)};
    *v = vec_add(vec_multiply(radius, unit), center);

    list_add(c, v);
  }

  return body_init(c, mass, color);
}

/** Return a list of points representing the pacman shape.
 *
 * @param center a vector representing the center of the pacman
 * @return list of vectors representing points of pacman object.
 */
list_t *get_pacman(vector_t center) {
  list_t *points = list_init(BALL_RADIUS, free);
  vector_t *vec = malloc(sizeof(vector_t));
  assert(vec);

  for (double i = (WEDGE_ANGLE / 2); i <= (2 * M_PI) - (WEDGE_ANGLE / 2);
       i += INCREMENT_ANGLE) {
    vector_t *new_vec = malloc(sizeof(vector_t));
    assert(new_vec);

    vector_t angle = {cos(i), sin(i)};
    *new_vec = vec_add(vec_multiply(PAC_RADIUS, angle), center);

    list_add(points, new_vec);
  }

  vector_t *new_vec = malloc(sizeof(vector_t));
  assert(new_vec);
  *new_vec = center;

  list_add(points, new_vec);
  return points;
}

// if the pacman eats the pellete
void pellets_collison(state_t *state) {
  vector_t pac = body_get_centroid(state->pacman);
  for (int i = 1; i < scene_bodies(state->scene); i++) {
    vector_t pellets = body_get_centroid(scene_get_body(state->scene, i));
    vector_t distance = vec_subtract(pac, pellets);
    if (fabs(distance.x) < PAC_RADIUS && fabs(distance.y) < PAC_RADIUS) {
      scene_remove_body(state->scene, i);
    }
  }
}

// when the pacman gets to the edge "wrap around"
// const vector_t WINDOW = (vector_t){.x = 1000, .y = 500};
void boundaries(state_t *state) {
  vector_t pac = body_get_centroid(scene_get_body(state->scene, 0));

  vector_t right = {0.0, pac.y};
  if (pac.x >= WINDOW.x + PAC_RADIUS) {
    body_set_centroid(state->pacman, right);
  }

  vector_t left = {WINDOW.x, pac.y};
  if (pac.x <= 0 - PAC_RADIUS) {
    body_set_centroid(state->pacman, left);
  }

  vector_t up = {pac.x, 0.0};
  if (pac.y >= WINDOW.y + PAC_RADIUS) {
    body_set_centroid(state->pacman, up);
  }

  vector_t down = {pac.x, WINDOW.y};
  if (pac.y <= 0 - PAC_RADIUS) {
    body_set_centroid(state->pacman, down);
  }
}

// this functions works on the keys
void on_key(char key, key_event_type_t type, double held_time, state_t *state) {
  double ACCELERATION = 100.0;
  const vector_t VELOCTIY_X = {100 + (held_time * ACCELERATION), 0};
  const vector_t VELOCTIY_Y = {0, 100 + (held_time * ACCELERATION)};

  if (type == KEY_PRESSED) {
    switch (key) {
    case UP_ARROW:
      body_set_velocity(state->pacman, VELOCTIY_Y);
      body_set_rotation(state->pacman, (M_PI / 2));
      break;
    case DOWN_ARROW:
      body_set_velocity(state->pacman, vec_negate(VELOCTIY_Y));
      body_set_rotation(state->pacman, -(M_PI / 2));
      break;
    case RIGHT_ARROW:
      body_set_velocity(state->pacman, VELOCTIY_X);
      body_set_rotation(state->pacman, 0);
      break;
    case LEFT_ARROW:
      body_set_velocity(state->pacman, vec_negate(VELOCTIY_X));
      body_set_rotation(state->pacman, (M_PI));
      break;
    }
  } else if (type == KEY_RELEASED) {
    body_set_velocity(state->pacman, VEC_ZERO);
  }
}

void emscripten_main(state_t *state) {

  boundaries(state);

  double change = time_since_last_tick();
  body_tick(state->pacman, change);
  sdl_on_key((void *)on_key);

  pellets_collison(state);
  if (scene_bodies(state->scene) == 1) {
    scene_remove_body(state->scene, 0);
  }
  sdl_render_scene(state->scene);
}

void emscripten_free(state_t *state) {
  free(state->scene);
  free(state->pacman);
  free(state);
}
