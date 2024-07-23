#include "list.h"
#include "polygon.h"
#include "sdl_wrapper.h"
#include "state.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// constants for sizing of star
const double PI = 3.14159265359;
const size_t RADIUS = 40;
// constants for rotation rate and initial velocity
const vector_t initV = {1, 0};
const double rotate = 0.01;
// constants for window display
const vector_t ORIGIN = (vector_t){.x = 0, .y = 0};
const vector_t WINDOW = (vector_t){.x = 1000, .y = 500};
const vector_t START = (vector_t){.x = 100, .y = 460};
// GRAVITY  CONSTANT
const double GRAV = -0.098;

typedef struct state {
  list_t *stars;
} state_t;

// used isha's code to create a star from last week
// passing in all the fields that need to be randomized or changed
polygon_t *create_star(size_t points, double R, double G, double B) {
  list_t *coordinates = list_init(points);
  for (int i = 0; i < points; i++) {
    vector_t *point = malloc(sizeof(vector_t));
    double angle = (PI / (points / 2)) * i;
    if (i % 2 == 0) { // outside points
      point->x = START.x + RADIUS * cos(angle);
      point->y = START.y + RADIUS * sin(angle);
    } else {
      point->x = START.x + RADIUS / 2 * cos(angle);
      point->y = START.y + RADIUS / 2 * sin(angle);
    }
    list_add(coordinates, point);
  }
  polygon_t *star = polygon_init(coordinates, initV, rotate, R, G, B);
  return star;
}

// implements gravity with y-velocity and updates velocity as it moves across
// the screen
void update_star(polygon_t *star) {
  vector_t starCenter = polygon_centroid(star);
  polygon_rotate(star, rotate, starCenter);
  polygon_set_velocity(star, polygon_get_velocity_x(star),
                       polygon_get_velocity_y(star) + GRAV);
  vector_t velocity = {polygon_get_velocity_x(star),
                       polygon_get_velocity_y(star) + GRAV};
  polygon_translate(star, velocity);
  list_t *vertices = polygon_get_points(star);
  for (size_t i = 0; i < list_size(polygon_get_points(star)); i++) {
    vector_t *vec = list_get(vertices, i);
    // kept if statement below in case need to slow down x-velocity for some
    // reason, helpful code for later double xVal = vec->x; double xMax =
    // WINDOW.x;
    //     if (xVal > xMax || xVal < 0) {
    //         velocity.x = (-0.9) * velocity.x;
    //         polygon_translate(star, velocity);
    //         polygon_set_velocity(star, polygon_get_velocity_x(star),
    //         velocity.y);
    //   // return
    //   break;
    //   }
    double y_val = vec->y;
    double y_max = WINDOW.y;
    if (y_val >= y_max || y_val < 0) {
      velocity.y = (-0.9) * velocity.y;
      polygon_translate(star, velocity);
      polygon_set_velocity(star, polygon_get_velocity_x(star), velocity.y);
      // return;
      break;
    }
  }
}

// checking if star is off the screen and returning true or false. helper method
// called in main.
bool remove_star(polygon_t *star) {
  list_t *vertices = polygon_get_points(star);
  for (size_t i = 0; i < list_size(polygon_get_points(star)); i++) {
    vector_t *position = list_get(vertices, i);
    if (position->x > 0 && position->x < WINDOW.x && position->y > 0 &&
        position->y < WINDOW.y) {
      return false;
    }
  }
  return true;
}

// helper functions to randomize color and number of points
double random_color() { return (double)rand() / (double)RAND_MAX; }
double random_points() {
  //+4 so at least 4 points
  return ((rand() % 10) + 4);
}
double total_time = 0;

state_t *emscripten_init() {
  sdl_init(ORIGIN, WINDOW);
  state_t *state = malloc(sizeof(state_t));
  list_t *polygons = list_init(10);
  double R = random_color();
  double G = random_color();
  double B = random_color();
  list_add(polygons, create_star(random_points(), R, G, B));
  state->stars = polygons;
  return state;
}

void emscripten_main(state_t *state) {
  sdl_clear();
  for (size_t i = 0; i < list_size(state->stars); i++) {
    polygon_t *star = list_get(state->stars, i);
    update_star(star);
    sdl_draw_polygon(star, color_get_red(polygon_get_color(star)),
                     color_get_green(polygon_get_color(star)),
                     color_get_blue(polygon_get_color(star)));
    if (remove_star(star)) {
      polygon_free(star);
      list_remove(state->stars, i);
    }
  }
  double dt = time_since_last_tick();
  total_time += dt;
  if (total_time > 3) {
    total_time = 0;
    polygon_t *star = create_star(random_points(), random_color(),
                                  random_color(), random_color());
    list_add(state->stars, star);
  }
  sdl_show();
}

void emscripten_free(state_t *state) {
  for (size_t i = 0; i < list_size(state->stars); i++) {
    polygon_free(list_get(state->stars, i));
  }
  list_free(state->stars);
  free(state);
}