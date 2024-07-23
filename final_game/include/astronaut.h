#ifndef __ASTRONAUT_H
#define __ASTRONAUT_H

#include "asset.h"
#include "asset_cache.h"
#include "collision.h"
#include "forces.h"
#include "sdl_wrapper.h"
#include <stddef.h>

typedef struct astronaut {
  asset_t *asset;
  body_t *body;
  size_t health;
  int16_t points;
  bool powerup;
  double powerup_time;
  bool moving_up;
  bool moving_down;
  double move_time;
  double move_duration;
  vector_t original_position;

  bool increase_health;
  double coin_time;
  bool increase_health_2;
  double coin_time_2;

} astronaut_t;

astronaut_t *astronaut_init();

void astronaut_free(astronaut_t *astro);

#endif