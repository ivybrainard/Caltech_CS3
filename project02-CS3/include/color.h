#ifndef __COLOR_H__
#define __COLOR_H__

typedef struct color color_t;

/**
 * Initialize a color object.
 *
 * @param red the double value for red
 * @param green the double value for green
 * @param blue the double value for blue
 * @return a pointer to a color_t object
 */
color_t *color_init(double red, double green, double blue);

/**
 * Return the double value of the red component of a color.
 *
 * @param color a color with r, g, b values
 * @return the red component of the color object
 */
double color_get_red(color_t *color);

/**
 * Return the double value of the green component of a color.
 *
 * @param color a color with r, g, b values
 * @return the green component of the color object
 */
double color_get_green(color_t *color);

/**
 * Return the double value of the blue component of a color.
 *
 * @param color a color with r, g, b values
 * @return the blue component of the color object
 */
double color_get_blue(color_t *color);

/**
 * Randomly generate rgb values for color of polygon.
 *
 * @return a pointer to a color object with randomly generated rgb values
 */
color_t *get_color();

/**
 * Free memory allocated for the color object.
 *
 * @param color a color with r, g, b values
 */
void color_free(color_t *color);

#endif // #ifndef __COLOR_H__
