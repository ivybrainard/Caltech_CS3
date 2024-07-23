#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "body.h"

/**
 * A rigid body constrained to the plane.
 * Implemented as a polygon with uniform density.
 */
struct body {
  polygon_t *poly;
  double mass;
};

body_t *body_init(list_t *shape, double mass, rgb_color_t color) {
  body_t *body = malloc(sizeof(body_t));
  assert(body != NULL);
  body->poly = polygon_init(shape, VEC_ZERO, 0, color.r, color.g, color.b);
  body->mass = mass;
  return body;
}

void body_free(body_t *body) {
  polygon_free(body->poly);
  free(body);
}

list_t *body_get_shape(body_t *body) {
  list_t *points = polygon_get_points(body->poly);
  list_t *shape = list_init(list_size(points), free);
  size_t num_points = list_size(points);
  for (size_t i = 0; i < num_points; i++) {
    vector_t point = *(vector_t *)list_get(points, i);
    vector_t *copy = malloc(sizeof(vector_t));
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
  vector_t velocity = body_get_velocity(body);
  vector_t centroid = body_get_centroid(body);
  vector_t new_centroid = vec_add(centroid, vec_multiply(dt, velocity));
  body_set_centroid(body, new_centroid);
}

double body_get_mass(body_t *body) { return body->mass; }
