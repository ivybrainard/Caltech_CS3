#include "polygon.h"
#include <math.h>
#include <stdio.h>

/**
 * Computes the area of a polygon.
 * See: https://artofproblemsolving.com/wiki/index.php/Shoelace_Theorem.
 *@param polygon the list of vertices that make up the polygon,
 * listed in a counterclockwise direction. There is an edge between
 * each pair of consecutive vertices, plus one between the first and last.
 * @return the area of the polygon
 */
double polygon_area(vec_list_t *polygon) {
  double area = 0;
  for (size_t i = 0; i < polygon->length - 1; i++) {
    area += (polygon->arrayVector[i]->x) * (polygon->arrayVector[i + 1]->y);
    area -= (polygon->arrayVector[i]->y) * (polygon->arrayVector[i + 1]->x);
  }

  area += (polygon->arrayVector[polygon->length - 1]->x) *
          (polygon->arrayVector[0]->y);
  area -= (polygon->arrayVector[polygon->length - 1]->y) *
          (polygon->arrayVector[0]->x);

  area = area / 2.0;
  return area;
}

/**
 * Computes the center of mass of a polygon.
 * See https://en.wikipedia.org/wiki/Centroid#Of_a_polygon.
 *
 * @param polygon the list of vertices that make up the polygon,
 * listed in a counterclockwise direction. There is an edge between
 * each pair of consecutive vertices, plus one between the first and last.
 * @return the centroid of the polygon
 */
vector_t polygon_centroid(vec_list_t *polygon) {

  vector_t list = VEC_ZERO;

  double xCompoent = 0.0;
  double yCompoent = 0.0;

  for (size_t i = 0; i < polygon->length - 1; i++) {
    xCompoent +=
        (polygon->arrayVector[i]->x + polygon->arrayVector[i + 1]->x) *
        ((polygon->arrayVector[i]->x * polygon->arrayVector[i + 1]->y) -
         (polygon->arrayVector[i + 1]->x * polygon->arrayVector[i]->y));
    yCompoent +=
        (polygon->arrayVector[i]->y + polygon->arrayVector[i + 1]->y) *
        ((polygon->arrayVector[i]->x * polygon->arrayVector[i + 1]->y) -
         (polygon->arrayVector[i + 1]->x * polygon->arrayVector[i]->y));
  }
  xCompoent += (polygon->arrayVector[polygon->length - 1]->x +
                polygon->arrayVector[0]->x) *
               ((polygon->arrayVector[polygon->length - 1]->x *
                 polygon->arrayVector[0]->y) -
                (polygon->arrayVector[0]->x *
                 polygon->arrayVector[polygon->length - 1]->y));
  yCompoent += (polygon->arrayVector[polygon->length - 1]->y +
                polygon->arrayVector[0]->y) *
               ((polygon->arrayVector[polygon->length - 1]->x *
                 polygon->arrayVector[0]->y) -
                (polygon->arrayVector[0]->x *
                 polygon->arrayVector[polygon->length - 1]->y));

  double area = polygon_area(polygon);
  xCompoent = xCompoent / (6 * area);
  yCompoent = yCompoent / (6 * area);

  list.x = xCompoent;
  list.y = yCompoent;
  return list;
}

/**
 * Translates all vertices in a polygon by a given vector.
 * Note: mutates the original polygon.
 *
 * @param polygon the list of vertices that make up the polygon
 * @param translation the vector to add to each vertex's position
 */
void polygon_translate(vec_list_t *polygon, vector_t translation) {
  for (size_t i = 0; i < polygon->length; i++) {
    vector_t vector = vec_add(*polygon->arrayVector[i], translation);
    polygon->arrayVector[i]->x = vector.x;
    polygon->arrayVector[i]->y = vector.y;
  }
}

/**
 * Rotates vertices in a polygon by a given angle about a given point.
 * Note: mutates the original polygon.
 *
 * @param polygon the list of vertices that make up the polygon
 * @param angle the angle to rotate the polygon, in radians.
 * A positive angle means counterclockwise.
 * @param point the point to rotate around
 */
void polygon_rotate(vec_list_t *polygon, double angle, vector_t point) {
  polygon_translate(polygon, vec_negate(point));
  for (size_t i = 0; i < polygon->length; i++) {
    vector_t vectorRotated = vec_rotate(*polygon->arrayVector[i], angle);
    polygon->arrayVector[i]->x = vectorRotated.x;
    polygon->arrayVector[i]->y = vectorRotated.y;
  }
  polygon_translate(polygon, point);
}