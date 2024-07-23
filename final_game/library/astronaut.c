#include "astronaut.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const size_t max = 5;
astronaut_t *astronaut_init() {
  astronaut_t *astro = malloc(sizeof(astronaut_t));
  assert(astro != NULL);
  astro->health = max;
  astro->points = 0;
  astro->powerup = false;
  astro->powerup_time = 0;
  astro->moving_up = false;
  astro->moving_down = false;
  astro->move_duration = 2.5;

  astro->increase_health = false;
  astro->coin_time = 0;

  return astro;
}

void astronaut_free(astronaut_t *astro) {
  asset_destroy(astro->asset);
  body_free(astro->body);
}