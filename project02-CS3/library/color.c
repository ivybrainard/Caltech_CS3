#include "color.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

const double COLOR = 255; // max value of each rgb value

typedef struct color {
  double red;
  double green;
  double blue;
} color_t;

color_t *color_init(double red, double green, double blue) {
  color_t *color = malloc(sizeof(color_t));
  assert(color);

  color->red = red;
  color->green = green;
  color->blue = blue;

  return color;
}

double color_get_red(color_t *color) { return color->red; }

double color_get_green(color_t *color) { return color->green; }

double color_get_blue(color_t *color) { return color->blue; }

color_t *get_color() {
  double r = (double)rand();
  double g = (double)rand();
  double b = (double)rand();

  r = fmod(r, COLOR) / COLOR;
  g = fmod(g, COLOR) / COLOR;
  b = fmod(b, COLOR) / COLOR;

  return color_init(r, g, b);
}

void color_free(color_t *color) { free(color); }
