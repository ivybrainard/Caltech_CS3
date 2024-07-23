#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "body.h"

struct body {
  polygon_t *poly;

  double mass;

  vector_t force;
  vector_t impulse;
  bool removed;

  void *info;
  free_func_t info_freer;
};

polygon_t *body_get_polygon(body_t *body) { return body->poly; }

void *body_get_info(body_t *body) { return body->info; }

body_t *body_init_with_info(list_t *shape, double mass, rgb_color_t color,
                            void *info, free_func_t info_freer) {
  body_t *body = malloc(sizeof(body_t));
  assert(body != NULL);
  body->poly = polygon_init(shape, VEC_ZERO, 0, color.r, color.g, color.b);
  assert(mass >= 0);
  body->mass = mass;
  body->force = VEC_ZERO;
  body->impulse = VEC_ZERO;
  body->removed = false;
  body->info = info;
  body->info_freer = info_freer;
  return body;
}

body_t *body_init(list_t *shape, double mass, rgb_color_t color) {
  return body_init_with_info(shape, mass, color, NULL, NULL);
}

void body_free(body_t *body) {
  polygon_free(body->poly);
  if (body->info_freer != NULL) {
    body->info_freer(body->info);
  }
  free(body);
}

list_t *body_get_shape(body_t *body) {
  list_t *points = polygon_get_points(body->poly);
  list_t *shape = list_init(list_size(points), free);
  size_t num_points = list_size(points);
  for (size_t i = 0; i < num_points; i++) {
    vector_t point = *(vector_t *)list_get(points, i);
    vector_t *copy = malloc(sizeof(vector_t));
    assert(copy != NULL);
    *copy = (vector_t){point.x, point.y};
    list_add(shape, copy);
  }
  return shape;
}

vector_t body_get_centroid(body_t *body) {
  return polygon_centroid(body->poly);
}

vector_t body_get_velocity(body_t *body) {
  vector_t velocity = {polygon_get_velocity_x(body->poly),
                       polygon_get_velocity_y(body->poly)};
  return velocity;
}

rgb_color_t *body_get_color(body_t *body) {
  return polygon_get_color(body->poly);
}

void body_set_color(body_t *body, rgb_color_t *col) {
  polygon_set_color(body->poly, col);
}

void body_set_centroid(body_t *body, vector_t x) {
  vector_t translate = vec_subtract(x, body_get_centroid(body));
  polygon_translate(body->poly, translate);
}

void body_set_velocity(body_t *body, vector_t v) {
  polygon_set_velocity(body->poly, v.x, v.y);
}

double body_get_rotation(body_t *body) {
  return polygon_get_rotation(body->poly);
}

void body_set_rotation(body_t *body, double angle) {
  polygon_rotate(body->poly, angle, body_get_centroid(body));
}

void body_tick(body_t *body, double dt) {
  vector_t old_velocity = body_get_velocity(body);
  vector_t new_velocity =
      vec_add(old_velocity, vec_multiply(dt / body->mass, body->force));
  new_velocity =
      vec_add(new_velocity, vec_multiply(1 / body->mass, body->impulse));
  body_set_velocity(body, new_velocity);
  vector_t avg_vel = vec_multiply(0.5, vec_add(old_velocity, new_velocity));
  vector_t old_centroid = vec_multiply(dt, avg_vel);
  vector_t new_centroid = vec_add(body_get_centroid(body), old_centroid);
  body_set_centroid(body, new_centroid);
  body->force = VEC_ZERO;
  body->impulse = VEC_ZERO;
}

double body_get_mass(body_t *body) { return body->mass; }

void body_add_force(body_t *body, vector_t force) {
  body->force = vec_add(body->force, force);
}

void body_add_impulse(body_t *body, vector_t impulse) {
  body->impulse = vec_add(body->impulse, impulse);
}

void body_remove(body_t *body) { body->removed = true; }

bool body_is_removed(body_t *body) { return body->removed; }
