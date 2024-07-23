#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "collision.h"
#include "forces.h"
#include "sdl_wrapper.h"

const double WEDGE_ANGLE = 3.6 * M_PI / 3;
const double INCREMENT_ANGLE = 0.1;
const double RADIUS = 40;
const double BULLET_RADIUS = 10;

const vector_t MIN = {0, 0};
const vector_t MAX = {1000, 500};

const vector_t USER_CENTER = {500, 30};
const vector_t INVADER_BULLET_VEL = {0, -200};

const double MASS = 5;
const double BULLET_MASS = 10;

const size_t SPAWN_TIME = 200; // number of frames in between new shapes
const size_t SHOOT_TIME = 2;

const double resting_speed = 300;
const double ACCEL = 100;

const double OUTER_RADIUS = 60;
const double INNER_RADIUS = 25;

const size_t SHIP_NUM_POINTS = 20;

const rgb_color_t invader_color = (rgb_color_t){0.2, 0.2, 0.3};
const rgb_color_t user_color = (rgb_color_t){0.1, 0.9, 0.2};

// constants to create invaders
const size_t NUM_ROWS = 3;
const size_t Y_START = 390;
const size_t Y_SPACE = 10;
const size_t INVADERS_PER_ROW = 8;
const size_t X_START = 100;
const size_t X_SPACE = 15;

const size_t OFFSET = 3;
const size_t CIRC_NPOINTS = 4;

struct state {
  scene_t *scene;
  size_t invader_count;
  bool game_over;
  body_t *player;
};

/** Make a circle-shaped body object.
 *
 * @param center a vector representing the center of the body.
 * @param radius the radius of the circle
 * @param mass the mass of the body
 * @param color the color of the circle
 * @return pointer to the circle-shaped body
 */
body_t *make_bullet(vector_t center, double radius, double mass,
                    rgb_color_t color, void *info) {
  list_t *c = list_init(CIRC_NPOINTS, free);
  for (size_t i = 0; i < CIRC_NPOINTS; i++) {
    double angle = 2 * M_PI * i / CIRC_NPOINTS;
    vector_t *v = malloc(sizeof(*v));
    *v = (vector_t){center.x + radius * cos(angle),
                    center.y + radius * sin(angle)};
    list_add(c, v);
  }
  return body_init_with_info(c, mass, color, info, NULL);
}

/** Return a list of points representing the invader shape.
 *
 * @param center a vector representing the center of the invader
 * @return list of vectors representing points of invader object.
 */
list_t *make_invader(vector_t center, double wedge) {
  list_t *points = list_init(10, free);
  vector_t *vec = malloc(sizeof(vector_t));
  assert(vec);

  double x;
  double y;
  for (double i = (wedge / 2); i <= (2 * M_PI) - (wedge / 2);
       i += INCREMENT_ANGLE) {
    vector_t *new_vec = malloc(sizeof(vector_t));
    assert(new_vec);

    x = RADIUS * cos(i) + center.x;
    y = RADIUS * sin(i) + center.y;

    *new_vec = (vector_t){x, y};
    list_add(points, new_vec);
  }

  vector_t *new_vec = malloc(sizeof(vector_t));
  assert(new_vec);
  *new_vec = center;
  list_add(points, new_vec);

  return points;
}

list_t *make_ship(double outer_radius, double inner_radius) {
  vector_t center = VEC_ZERO;
  center.y += inner_radius;
  list_t *c = list_init(SHIP_NUM_POINTS, free);
  for (size_t i = 0; i < SHIP_NUM_POINTS; i++) {
    double angle = 2 * M_PI * i / SHIP_NUM_POINTS;
    vector_t *v = malloc(sizeof(*v));
    *v = (vector_t){center.x + outer_radius * cos(angle),
                    center.y + inner_radius * sin(angle)};
    list_add(c, v);
  }
  return c;
}

/**
 * Wrap object around other side of screen display if it reaches any edge of
 the
 * display.
 *
 * @param body the body object representing an invader
 * @param amount amount to offset in the y-direction
 */
void wrap_edges(body_t *body, double offset) {
  vector_t velocity = body_get_velocity(body);
  vector_t centroid = body_get_centroid(body);
  if (centroid.x + RADIUS > MAX.x && velocity.x > 0) {
    body_set_centroid(body, (vector_t){centroid.x, centroid.y - offset});
    body_set_velocity(body, vec_negate(velocity));
  } else if (centroid.x - RADIUS < MIN.x && velocity.x < 0) {
    body_set_centroid(body, (vector_t){centroid.x, centroid.y - offset});
    body_set_velocity(body, vec_negate(velocity));
  }
}

/**
 * Wrap object around other side of screen display if it reaches any edge of
 the
 * display.
 *
 * @param body the body object representing the user
 */
void user_wrap_edges(body_t *body) {
  vector_t centroid = body_get_centroid(body);
  if (centroid.x + RADIUS > MAX.x) {
    body_set_centroid(body, (vector_t){MIN.x + RADIUS, centroid.y});
  } else if (centroid.x - RADIUS < MIN.x) {
    body_set_centroid(body, (vector_t){MAX.x - RADIUS, centroid.y});
  }
}

// calculate new locations after wrap-around for invaders
void wrap_invaders(state_t *state) {
  for (size_t i = 0; i < state->invader_count; i++) {
    body_t *invader = scene_get_body(state->scene, i);
    wrap_edges(invader, (RADIUS + 10) * OFFSET);
  }
}

/**
 * Check conditions to see if game is over. Game is over if there are
 * no more invaders (win), the invaders reach the bottom of the screen (loss)
 * or the user is hit by an invader bullet (loss).
 *
 * @param state a pointer to a state object representing the current demo
 state
 */
bool game_over(state_t *state) {
  if (state->invader_count == 1) {
    return true;
  }
  size_t num_bodies = scene_bodies(state->scene);
  for (size_t i = 0; i < num_bodies; i++) {
    body_t *body = scene_get_body(state->scene, i);
    if (strcmp(body_get_info(body), "invader_bullet") == 0) {
      if (find_collision(body, state->player)) {
        return true;
      }
    } else if (strcmp(body_get_info(body), "invader") == 0 &&
               body_get_centroid(body).y - RADIUS < MIN.y) {
      return true;
    }
  }
  return false;
}

/**
 * Move ship on display screen or shoots bullet based based on key pressed.
 *
 * @param key the character of the key pressed
 * @param type event type connected to key
 * @param held_time double value representing the amount of time the key is
 held
 * down
 * @param scene the scene holding the bodies
 */
void on_key(char key, key_event_type_t type, double held_time, state_t *state) {
  if (type == KEY_PRESSED) {
    body_t *player = state->player;
    switch (key) {
    case LEFT_ARROW:
      body_set_velocity(player, (vector_t){-resting_speed, 0});
      break;
    case RIGHT_ARROW:
      body_set_velocity(player, (vector_t){resting_speed, 0});
      break;
    case SPACE_BAR: {
      vector_t bullet_center = body_get_centroid(player);
      body_t *bullet = make_bullet(bullet_center, BULLET_RADIUS, BULLET_MASS,
                                   user_color, "player_bullet");
      body_set_velocity(bullet, vec_negate(INVADER_BULLET_VEL));
      scene_add_body(state->scene, bullet);
      break;
    }
    default:
      break;
    }
  }
}

// initialize the invaders at start of game
void invader_init(state_t *state) {
  for (int i = 0; i < NUM_ROWS; i++) {
    double y = Y_START + i * (RADIUS + Y_SPACE);
    for (int j = 0; j < INVADERS_PER_ROW; j++) {
      double x = X_START + j * (RADIUS * 2 + X_SPACE);
      list_t *invader_points = make_invader((vector_t){x, y}, WEDGE_ANGLE);
      body_t *invader = body_init_with_info(invader_points, MASS, invader_color,
                                            "invader", NULL);

      body_set_velocity(invader, (vector_t){100, 0});
      body_set_rotation(invader, 3 * M_PI / 2);

      scene_add_body(state->scene, invader);
      state->invader_count++;
    }
  }
}

/** Chooses a random invader out of the remaining invaders
 * in the demo to shoot a bullet at the user.
 *
 * @param state a pointer to a state object representing the current demo
 state
 */
void invader_shoot_bullet(state_t *state) {
  if (state->invader_count > 0) {
    size_t random_index = (size_t)rand() % state->invader_count;
    size_t invader_count = 0;
    for (size_t i = 0; i < scene_bodies(state->scene); i++) {
      body_t *body = scene_get_body(state->scene, i);
      if (strcmp(body_get_info(body), "invader") == 0) {
        if (invader_count == random_index) {
          vector_t bullet_center = body_get_centroid(body);
          body_t *bullet =
              make_bullet(bullet_center, BULLET_RADIUS, BULLET_MASS,
                          invader_color, "invader_bullet");
          body_set_velocity(bullet, INVADER_BULLET_VEL);
          scene_add_body(state->scene, bullet);
          break;
        }
        invader_count++;
      }
    }
  }
}

state_t *emscripten_init() {
  sdl_init(MIN, MAX);
  state_t *state = malloc(sizeof(state_t));
  assert(state);
  state->scene = scene_init();
  list_t *player_points = make_ship(OUTER_RADIUS, INNER_RADIUS);
  state->player =
      body_init_with_info(player_points, MASS, user_color, "player", NULL);
  body_set_centroid(state->player, USER_CENTER);
  scene_add_body(state->scene, state->player);
  state->invader_count = 1;
  invader_init(state);
  return state;
}

bool emscripten_main(state_t *state) {
  static double spawn_time = 0;
  static double shoot_time = 0;
  wrap_invaders(state);
  sdl_on_key((key_handler_t)on_key);

  double dt = time_since_last_tick();
  spawn_time += dt;
  shoot_time += dt;

  if (spawn_time > SPAWN_TIME) {
    invader_init(state);
    spawn_time = 0;
  }
  if (shoot_time > SHOOT_TIME) {
    invader_shoot_bullet(state);
    shoot_time = 0;
  }

  size_t num_bodies = scene_bodies(state->scene);
  for (size_t i = 0; i < num_bodies; i++) {
    body_t *body = scene_get_body(state->scene, i);
    vector_t centroid = body_get_centroid(body);

    if (strcmp(body_get_info(body), "player_bullet") == 0 ||
        strcmp(body_get_info(body), "invader_bullet") == 0) {
      if (centroid.y > MAX.y || centroid.y < MIN.y)
        scene_remove_body(state->scene, i);
    }

    if (strcmp(body_get_info(body), "player_bullet") == 0) {
      for (size_t j = 0; j < num_bodies; j++) {
        body_t *invader = scene_get_body(state->scene, j);
        if (strcmp(body_get_info(invader), "invader") == 0 &&
            find_collision(body, invader)) {
          create_destructive_collision(state->scene, body, invader);
          state->invader_count--;
        }
      }
    }
  }

  scene_tick(state->scene, dt);
  sdl_render_scene(state->scene, state->player);
  user_wrap_edges(state->player);
  if (game_over(state)) {
    return true;
  }
  return false;
}

void emscripten_free(state_t *state) {
  scene_free(state->scene);
  free(state->player);
  free(state);
}