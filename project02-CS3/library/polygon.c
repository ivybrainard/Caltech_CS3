
#include "polygon.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct polygon {
  list_t *points;
  vector_t vel;
  double rot_speed;
  color_t *color;
} polygon_t;

polygon_t *polygon_init(list_t *points, vector_t initial_velocity,
                        double rotation_speed, double red, double green,
                        double blue) {
  polygon_t *poly = malloc(sizeof(polygon_t));
  poly->color = color_init(red, green, blue);
  poly->points = points;
  poly->vel = initial_velocity;
  poly->rot_speed = rotation_speed;
  return poly;
}

list_t *polygon_get_points(polygon_t *polygon) { return polygon->points; }

void polygon_move(polygon_t *polygon, double time_elapsed) {
  polygon_translate(polygon, polygon->vel);
  polygon_rotate(polygon, (polygon->rot_speed) * time_elapsed,
                 polygon_centroid(polygon));
  polygon_set_velocity(polygon, polygon_get_velocity_x(polygon),
                       polygon_get_velocity_y(polygon) - 9.8);
}

void polygon_set_velocity(polygon_t *polygon, double v_x, double v_y) {
  polygon->vel.x = v_x;
  polygon->vel.y = v_y;
}

void polygon_free(polygon_t *polygon) {
  list_free(polygon->points);
  color_free(polygon->color);
  free(polygon);
}

double polygon_get_velocity_x(polygon_t *polygon) { return polygon->vel.x; }

double polygon_get_velocity_y(polygon_t *polygon) { return polygon->vel.y; }

double polygon_area(polygon_t *polygon) {
  double area = 0;
  size_t size = list_size(polygon->points);
  for (size_t i = 0; i < size - 1; i++) {
    vector_t *vec_i = (vector_t *)(list_get(polygon->points, i));
    vector_t *vec_i1 = (vector_t *)(list_get(polygon->points, i + 1));
    area += (vec_i->x) * (vec_i1->y);
    area -= (vec_i->y) * (vec_i1->x);
  }

  vector_t *vec_s1 = (vector_t *)(list_get(polygon->points, size - 1));
  vector_t *vec_0 = (vector_t *)(list_get(polygon->points, 0));

  area += (vec_s1->x) * (vec_0->y);
  area -= (vec_s1->y) * (vec_0->x);

  area = area / 2.0;
  return area;
}

vector_t polygon_centroid(polygon_t *polygon) {
  size_t size = list_size(polygon->points);

  vector_t list = VEC_ZERO;

  double x_component = 0.0;
  double y_component = 0.0;

  for (size_t i = 0; i < size - 1; i++) {
    vector_t *vec_i = (vector_t *)(list_get(polygon->points, i));
    vector_t *vec_i1 = (vector_t *)(list_get(polygon->points, i + 1));
    x_component += (vec_i->x + vec_i1->x) *
                   ((vec_i->x * vec_i1->y) - (vec_i1->x * vec_i->y));
    y_component += (vec_i->y + vec_i1->y) *
                   ((vec_i->x * vec_i1->y) - (vec_i1->x * vec_i->y));
  }

  vector_t *vec_s1 = (vector_t *)(list_get(polygon->points, size - 1));
  vector_t *vec_0 = (vector_t *)(list_get(polygon->points, 0));
  x_component += (vec_s1->x + vec_0->x) *
                 ((vec_s1->x * vec_0->y) - (vec_0->x * vec_s1->y));
  y_component += (vec_s1->y + vec_0->y) *
                 ((vec_s1->x * vec_0->y) - (vec_0->x * vec_s1->y));

  double area = polygon_area(polygon);
  x_component = x_component / (6 * area);
  y_component = y_component / (6 * area);

  list.x = x_component;
  list.y = y_component;
  return list;
}

void polygon_translate(polygon_t *polygon, vector_t translation) {
  size_t size = list_size(polygon->points);
  for (size_t i = 0; i < size; i++) {
    vector_t *vec_i = (vector_t *)(list_get(polygon->points, i));
    vector_t vector = vec_add(*vec_i, translation);
    vec_i->x = vector.x;
    vec_i->y = vector.y;
  }
}

void polygon_rotate(polygon_t *polygon, double angle, vector_t point) {
  size_t size = list_size(polygon->points);
  polygon_translate(polygon, vec_negate(point));
  for (size_t i = 0; i < size; i++) {
    vector_t *vec_i = (vector_t *)(list_get(polygon->points, i));
    vector_t vector_rotated = vec_rotate(*vec_i, angle);
    vec_i->x = vector_rotated.x;
    vec_i->y = vector_rotated.y;
  }
  polygon_translate(polygon, point);
}

color_t *polygon_get_color(polygon_t *polygon) { return polygon->color; }
