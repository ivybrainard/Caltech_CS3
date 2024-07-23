#ifndef __POLYGON_H__
#define __POLYGON_H__

#include "color.h"
#include "list.h"

typedef struct polygon polygon_t;

/**
 * Initialize a polygon object given a list of vertices.
 *
 * @param points the list of vertices that make up the polygon
 * @param initial_velocity a vector representing the initial velocity of the
 * polygon
 * @param rotation_speed the rotation angle of the polygon per unit time
 * @param red double value between 0 and 1 representing the red of the polygon
 * @param green double value between 0 and 1 representing the green of the
 * polygon
 * @param blue double value between 0 and 1 representing the blue of the polygon
 * @return a polygon object pointer
 */
polygon_t *polygon_init(list_t *points, vector_t initial_velocity,
                        double rotation_speed, double red, double green,
                        double blue);

/**
 * Return the list of vectors representing the vertices of the polygon.
 *
 * @param polygon the pointer to the polygon
 * @return a list of vectors
 */
list_t *polygon_get_points(polygon_t *polygon);

/**
 * Translate and rotate the polygon then update velocity based on gravity.
 *
 * @param polygon the pointer to the polygon
 * @param time_elapsed time/# of frames elapsed since the last tick
 */
void polygon_move(polygon_t *polygon, double time_elapsed);

/**
 * Set the x and y components of a polygon's velocity vector.
 *
 * @param polygon the pointer to the polygon
 * @param v_x x component of velocity to set
 * @param v_y y component of velocity to set
 */
void polygon_set_velocity(polygon_t *polygon, double v_x, double v_y);

/**
 * Free memory allocated for object associated with a polygon.
 *
 * @param polygon the pointer to the polygon
 */
void polygon_free(polygon_t *polygon);

/**
 * Return x-component of polygon's velocity vector.
 *
 * @param polygon the pointer to the polygon
 * @return the x-velocity of a polygon
 */
double polygon_get_velocity_x(polygon_t *polygon);

/**
 * Return y-component of polygon's velocity vector.
 *
 * @param polygon the pointer to the polygon
 * @return the y-velocity of a polygon
 */
double polygon_get_velocity_y(polygon_t *polygon);

/**
 * Computes the area of a polygon.
 * See https://en.wikipedia.org/wiki/Shoelace_formula#Statement.
 *
 * @param polygon the pointer to the polygon,
 * listed in a counterclockwise direction. There is an edge between
 * each pair of consecutive vertices, plus one between the first and last.
 * @return the area of the polygon
 */
double polygon_area(polygon_t *polygon);

/**
 * Computes the center of mass of a polygon.
 * See https://en.wikipedia.org/wiki/Centroid#Of_a_polygon.
 *
 * @param polygon the pointer to the polygon,
 * listed in a counterclockwise direction. There is an edge between
 * each pair of consecutive vertices, plus one between the first and last.
 * @return the centroid of the polygon
 */
vector_t polygon_centroid(polygon_t *polygon);

/**
 * Translates all vertices in a polygon by a given vector.
 * Note: mutates the original polygon.
 *
 * @param polygon the pointer to the polygon
 * @param translation the vector to add to each vertex's position
 */
void polygon_translate(polygon_t *polygon, vector_t translation);

/**
 * Rotates vertices in a polygon by a given angle about a given point.
 *
 * @param polygon the pointer to the polygon
 * @param angle the angle to rotate the polygon, in radians.
 * A positive angle means counterclockwise.
 * @param point the point to rotate around
 */
void polygon_rotate(polygon_t *polygon, double angle, vector_t point);

/**
 * Return the polygon's color.
 *
 * @param polygon the list of vertices that make up the polygon
 * @return the color_t struct representing the color
 */
color_t *polygon_get_color(polygon_t *polygon);

#endif // #ifndef __POLYGON_H__
