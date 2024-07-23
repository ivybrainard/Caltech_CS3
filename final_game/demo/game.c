#include "asset.h"
#include "asset_cache.h"
#include "astronaut.h"
#include "collision.h"
#include "forces.h"
#include "sdl_wrapper.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const double PATHS[] = {90, 210, 330};
const vector_t MIN = {0, 0};
const vector_t MAX = {600, 400};
const vector_t RESET_POS = {15, 210};
const vector_t RESET_POS_2 = {50, 330};
const vector_t OFF_SCREEN = {0, 100000};
const vector_t BASE_OBJ_VEL = {-30, 0};
const size_t NUM_BUTTONS = 9;
const size_t MAX_HEALTH = 5;
const double STAR_ROT = M_PI / 6;
const double ROCKET_ROT = 11 * M_PI / 6;
const size_t CHARACTER_SIZE = 70;
const size_t CHARACTER_SIZE1 = 150;
const size_t REG_POINTS = 1;
const size_t EXTRA_POINTS = 10;
const double STAR_ROCKET_SHIFT = 50.98076211;
const double STAR_SHIFT = 20;
const double INCREASED_VELOCITY_FACTOR = 1.0001;
const double DECREASED_JUMP_FACTOR = 1.001;
const size_t METEOR_INDEX = 4;
const size_t STAR_INDEX_1 = 5;
const size_t STAR_INDEX_2 = 9;
const size_t COIN_INDEX_1 = 10;
const size_t COIN_INDEX_2 = 15;
const size_t EXTRA_COIN_1 = 16;
const size_t EXTRA_COIN_2 = 17;
const size_t POINTS_UPPER = 30;
const size_t NUM_PLAYERS = 2;
// plays from the first available channel
const size_t CHANNEL = -1;
// constants for font
const rgb_color_t WHITE = (rgb_color_t){255, 255, 255};
const size_t FONT_SIZE1 = 50;
const size_t FONT_SIZE2 = 18;
// messages
const char *START_MESSAGE1 = "Welcome to Space Escape!";
const char *START_MESSAGE2 =
    "Navigate through all the obstacles and collect coins in all of the";
const char *START_MESSAGE3 = "planets until you get back to Earth!";
const char *LIFE_MESSAGE1 = "Player 1 you gain a life!";
const char *LIFE_MESSAGE2 = "Player 2 you gain a life!";
const char *PLAYER1_MESSAGE = "Player 1";
const char *PLAYER2_MESSAGE = "Player 2";
const char *WINNER_MESSAGE1 = "Player one wins!";
const char *WINNER_MESSAGE2 = "Player two wins!";
const char *WINNER_TIE_MESSAGE = "Both Players win!";
const char *CHOOSE_TEXT_MESSAGE = "Choose your player!";
const char *INSTRUCTIONS_MESSAGE1 = "Use the arrow keys to move Player 1";
const char *INSTRUCTIONS_MESSAGE2 = "Use the A,W,S,D keys to move Player 2";

const char *PLAYER_ONE = "1";
const char *PLAYER_TWO = "2";

const vector_t positions_health[] = {(vector_t){550, 375}, (vector_t){530, 375},
                                     (vector_t){510, 375}, (vector_t){490, 375},
                                     (vector_t){470, 375}};

const vector_t positions_health_2[] = {
    (vector_t){50, 375}, (vector_t){70, 375}, (vector_t){90, 375},
    (vector_t){110, 375}, (vector_t){130, 375}};

const size_t INTERVAL = 20;
const size_t POWERUP_INTERVAL = 7;
const double OUTER_RADIUS = 15;
const double INNER_RADIUS = 15;
const double OBS_HEIGHT = 30;
const double OBS_WIDTH = 30;
const double STAR_HEIGHT = 50;
const double STAR_WIDTH = 30;
const vector_t OBS_SPACING = {110, 150};
const vector_t PLAYER_1_POS = {375, 300};
const vector_t PLAYER_1_POS_INITIAL = {300, 150};
const vector_t PLAYER_2_POS = {375, 100};
const size_t NUM_POINTS = 20;
const size_t NUM_OBSTACLES = 20;
const rgb_color_t OBS_COLOR = (rgb_color_t){0.2, 0.2, 0.3};
const rgb_color_t ASTRONAUT_COLOR = (rgb_color_t){0.1, 0.9, 0.2};

// constants to create invaders
const int16_t H_STEP = 45;
const int16_t V_STEP = 120;
const size_t ROWS = 3;
const size_t NUM_BACKGROUND = 10;
const size_t CHANGE_INDEX = 2;
const size_t NUM_CHARACTERS = 50;

const char *HEART_PATH = "assets/heart.png";
const char *METEOR_PATH = "assets/meteor.png";
const char *COIN_PATH = "assets/coin.png";
const char *ROCKET_PATH = "assets/rocketship.png";
const char *STAR_PATH = "assets/shooting_star.png";
const char *EXTRA_COIN_PATH = "assets/coin_boost.png";
const char *BACKGROUNDS[] = {"assets/pink.png",
                             "assets/customization.png",
                             "assets/outer_space.png",
                             "assets/neptune.png",
                             "assets/uranus.png",
                             "assets/saturn.png",
                             "assets/jupiter.png",
                             "assets/mars.png",
                             "assets/earth_background.png",
                             "assets/game_over.png"};
const char *FONT = "assets/Roboto-Regular.ttf";
const char *FONT1 = "assets/Gaming-Font.ttf";
const char *CHARACTER_PATHS[] = {
    "assets/astronaut.png",     "assets/blue_character.png",
    "assets/pink_char.png",     "assets/purple_character.png",
    "assets/cat_character.png", "assets/green_character.png"};
const char *COIN_SOUND = "assets/coin_sound.wav";
const char *METEOR_SOUND = "assets/meteor.wav";
const char *GAME_OVER_SOUND = "assets/game_over.wav";
const char *BACKGROUND_SOUND = "assets/background_sound.wav";

void single_player_button_handler(state_t *state);
void dual_play_button_handler(state_t *state);
void pink_character(state_t *state);
void og_character(state_t *state);
void blue_character(state_t *state);
void green_character(state_t *state);
void start_button_handler(state_t *state);
void purple_character(state_t *state);
void cat_character(state_t *state);

struct state {
  list_t *obstacle_assets;
  list_t *obstacles;
  list_t *player_assets;
  scene_t *scene;
  astronaut_t *astro_1;
  astronaut_t *astro_2;
  list_t *backgrounds;
  size_t background_index;
  double time;
  list_t *button_assets;
  list_t *manual_buttons;
  asset_t *single_player_button;
  asset_t *dual_player_button;
  asset_t *start_button;
  bool is_customized;
  bool single;
  bool dual;
  bool sound;
};

typedef struct button_info {
  const char *image_path;
  const char *font_path;
  SDL_Rect image_box;
  SDL_Rect text_box;
  rgb_color_t text_color;
  const char *text;
  u_int8_t text_size;
  button_handler_t handler;
} button_info_t;

button_info_t button_templates[] = {
    {.image_path = "assets/single_player_button.png",
     .image_box = (SDL_Rect){50, 350, 200, 50},
     .handler = (void *)single_player_button_handler},
    {.image_path = "assets/dual_player_button.png",
     .image_box = (SDL_Rect){350, 350, 200, 50},
     .handler = (void *)dual_play_button_handler},
    {.image_path = "assets/start_button.png",
     .image_box = (SDL_Rect){400, 200, 200, 50},
     .handler = (void *)start_button_handler},
    {.image_path = "assets/pick_button.png",
     .image_box = (SDL_Rect){0, 120, 200, 50},
     .handler = (void *)pink_character},
    {.image_path = "assets/pick_button.png",
     .image_box = (SDL_Rect){0, 220, 200, 50},
     .handler = (void *)blue_character},
    {.image_path = "assets/pick_button.png",
     .image_box = (SDL_Rect){0, 320, 200, 50},
     .handler = (void *)og_character},
    {.image_path = "assets/pick_button.png",
     .image_box = (SDL_Rect){400, 320, 200, 50},
     .handler = (void *)green_character},
    {.image_path = "assets/pick_button.png",
     .image_box = (SDL_Rect){400, 220, 200, 50},
     .handler = (void *)cat_character},
    {.image_path = "assets/pick_button.png",
     .image_box = (SDL_Rect){400, 120, 200, 50},
     .handler = (void *)purple_character}};

asset_t *create_button_from_info(state_t *state, button_info_t info) {
  asset_t *image = NULL;
  if (info.image_path != NULL) {
    image = asset_make_image(info.image_path, info.image_box);
  }
  asset_t *text = NULL;
  if (info.text != NULL && info.font_path != NULL) {
    text = asset_make_text(info.font_path, info.text_box, info.text,
                           info.text_color, info.text_size);
  }
  asset_t *button_info = asset_make_button(info.image_box, (asset_t *)image,
                                           (asset_t *)text, info.handler);
  asset_cache_register_button(button_info);
  return button_info;
}

void single_player_button_handler(state_t *state) {
  if (state->dual) {
    return;
  } else {
    state->single = true;
    state->background_index++;
    body_set_centroid(state->astro_2->body, OFF_SCREEN);
    for (size_t i = 0; i < list_size(state->obstacles); i++) {
      body_set_velocity(list_get(state->obstacles, i), BASE_OBJ_VEL);
    }
    list_remove(state->player_assets, 1);
  }
}

void dual_play_button_handler(state_t *state) {
  if (state->single) {
    return;
  } else {
    state->dual = true;
    state->time = 0;
    state->background_index++;
    for (size_t i = 0; i < list_size(state->obstacles); i++) {
      body_set_velocity(list_get(state->obstacles, i), BASE_OBJ_VEL);
    }
  }
}

void start_button_handler(state_t *state) {
  load_sound(CHANNEL, BACKGROUND_SOUND);
  state->time = 0;
  state->background_index++;
  state->astro_1->health = MAX_HEALTH;
  state->astro_1->points = 0;
  state->astro_2->points = 0;
  state->astro_2->health = MAX_HEALTH;
  state->is_customized = true;
}

void og_character(state_t *state) {
  load_sound(CHANNEL, BACKGROUND_SOUND);
  state->is_customized = true;
  state->time = 0;
  state->background_index++;
}

void blue_character(state_t *state) {
  load_sound(CHANNEL, BACKGROUND_SOUND);
  state->is_customized = true;
  asset_t *blue_char =
      asset_make_image_with_body(CHARACTER_PATHS[1], state->astro_1->body);
  state->astro_1->asset = blue_char;
  list_remove(state->player_assets, 0);
  list_add(state->player_assets, blue_char);
  state->astro_1->health = MAX_HEALTH;
  state->astro_1->points = 0;
  state->time = 0;
  state->background_index++;
}

void cat_character(state_t *state) {
  load_sound(CHANNEL, BACKGROUND_SOUND);
  state->is_customized = true;
  asset_t *cat_char =
      asset_make_image_with_body(CHARACTER_PATHS[4], state->astro_1->body);
  state->astro_1->asset = cat_char;
  list_remove(state->player_assets, 0);
  list_add(state->player_assets, cat_char);
  state->astro_1->health = MAX_HEALTH;
  state->astro_1->points = 0;
  state->time = 0;
  state->background_index++;
}

void purple_character(state_t *state) {
  load_sound(CHANNEL, BACKGROUND_SOUND);
  state->is_customized = true;
  asset_t *purple_char =
      asset_make_image_with_body(CHARACTER_PATHS[3], state->astro_1->body);
  state->astro_1->asset = purple_char;
  list_remove(state->player_assets, 0);
  list_add(state->player_assets, purple_char);
  state->astro_1->health = MAX_HEALTH;
  state->astro_1->points = 0;
  state->time = 0;
  state->background_index++;
}

void pink_character(state_t *state) {
  load_sound(CHANNEL, BACKGROUND_SOUND);
  state->is_customized = true;
  asset_t *pink_char =
      asset_make_image_with_body(CHARACTER_PATHS[2], state->astro_1->body);
  state->astro_1->asset = pink_char;
  list_remove(state->player_assets, 0);
  list_add(state->player_assets, pink_char);
  state->astro_1->health = MAX_HEALTH;
  state->astro_1->points = 0;
  state->time = 0;
  state->background_index++;
}

void green_character(state_t *state) {
  load_sound(CHANNEL, BACKGROUND_SOUND);
  state->is_customized = true;
  asset_t *green_char =
      asset_make_image_with_body(CHARACTER_PATHS[5], state->astro_1->body);
  state->astro_1->asset = green_char;
  list_remove(state->player_assets, 0);
  list_add(state->player_assets, green_char);
  state->astro_1->health = MAX_HEALTH;
  state->astro_1->points = 0;
  state->time = 0;
  state->background_index++;
}

void create_buttons(state_t *state) {
  for (size_t i = 0; i < NUM_BUTTONS; i++) {
    button_info_t info = button_templates[i];
    asset_t *button = create_button_from_info(state, info);
    if (info.font_path == NULL) {
      if (strcmp(info.image_path, "assets/single_player_button.png") == 0) {
        state->single_player_button = button;
      } else if (strcmp(info.image_path, "assets/dual_player_button.png") ==
                 0) {
        state->dual_player_button = button;
      } else if (strcmp(info.image_path, "assets/start_button.png") == 0) {
        state->start_button = button;
      } else {
        list_add(state->manual_buttons, button);
      }
    }
  }
}

body_t *make_obstacle(size_t w, size_t h, vector_t center) {
  list_t *c = list_init(4, free);
  vector_t *v1 = malloc(sizeof(vector_t));
  assert(v1 != NULL);
  *v1 = (vector_t){0, 0};
  list_add(c, v1);
  vector_t *v2 = malloc(sizeof(vector_t));
  assert(v2 != NULL);
  *v2 = (vector_t){w, 0};
  list_add(c, v2);
  vector_t *v3 = malloc(sizeof(vector_t));
  assert(v3 != NULL);
  *v3 = (vector_t){w, h};
  list_add(c, v3);
  vector_t *v4 = malloc(sizeof(vector_t));
  assert(v4 != NULL);
  *v4 = (vector_t){0, h};
  list_add(c, v4);
  body_t *obstacle = body_init(c, 1, OBS_COLOR);
  body_set_centroid(obstacle, center);
  return obstacle;
}

body_t *make_astro(double outer_radius, double inner_radius, vector_t center,
                   char *info) {
  center.y += inner_radius;
  list_t *c = list_init(NUM_POINTS, free);
  for (size_t i = 0; i < NUM_POINTS; i++) {
    double angle = 2 * M_PI * i / NUM_POINTS;
    vector_t *v = malloc(sizeof(*v));
    assert(v != NULL);
    *v = (vector_t){center.x + inner_radius * cos(angle),
                    center.y + outer_radius * sin(angle)};
    list_add(c, v);
  }
  body_t *astronaut_char =
      body_init_with_info(c, 1, ASTRONAUT_COLOR, info, NULL);
  return astronaut_char;
}

void decrease_health(body_t *body, state_t *state) {
  if (state->is_customized) {
    char *player_num = (char *)body_get_info(body);
    if (!state->astro_1->powerup) {
      if (strcmp(PLAYER_ONE, player_num) == 0) {
        state->astro_1->health -= 1;
        load_sound(CHANNEL, METEOR_SOUND);
        if (state->astro_1->health == 0) {
          state->background_index = NUM_BACKGROUND - 1;
        }
        fprintf(stdout, "Player 1 has %zu lives!\n", state->astro_1->health);
      }
    }
    if (!state->astro_2->powerup) {
      if (strcmp(PLAYER_TWO, player_num) == 0) {
        state->astro_2->health -= 1;
        load_sound(CHANNEL, METEOR_SOUND);
        if (state->astro_2->health == 0) {
          state->background_index = NUM_BACKGROUND - 1;
        }
        fprintf(stdout, "Player 2 has %zu lives!\n", state->astro_2->health);
      }
    }
  }
}

void meteor_handler(body_t *body1, body_t *body2, vector_t axis, void *aux,
                    double force_const) {
  decrease_health(body1, (state_t *)aux);
}

void increase_points(body_t *body, state_t *state, size_t num_points) {
  if (state->is_customized) {
    char *player_num = (char *)body_get_info(body);
    if (strcmp(PLAYER_ONE, player_num) == 0) {
      state->astro_1->points += num_points;
      load_sound(CHANNEL, COIN_SOUND);
      if (state->astro_1->points > POINTS_UPPER &&
          state->astro_1->health < MAX_HEALTH) {
        state->astro_1->health++;
        state->astro_1->points -= POINTS_UPPER;
        state->astro_1->increase_health = true;
        state->astro_1->coin_time = 0;
      }
    }

    if (strcmp(PLAYER_TWO, player_num) == 0) {
      state->astro_2->points += num_points;
      load_sound(CHANNEL, COIN_SOUND);
      if (state->astro_2->points > POINTS_UPPER &&
          state->astro_2->health < MAX_HEALTH) {
        state->astro_2->health++;
        state->astro_2->points -= POINTS_UPPER;
        state->astro_2->increase_health = true;
        state->astro_2->coin_time = 0;
      }
    }
  }
}
void coin_handler(body_t *body1, body_t *body2, vector_t axis, void *aux,
                  double force_const) {
  increase_points(body1, (state_t *)aux, REG_POINTS);
}

void extra_coin(body_t *body1, body_t *body2, vector_t axis, void *aux,
                double force_const) {
  increase_points(body1, (state_t *)aux, EXTRA_POINTS);
}

void remove_obstacles(body_t *body, state_t *state) {
  if (state->is_customized) {
    char *player_num = body_get_info(body);
    if (strcmp(PLAYER_ONE, player_num) == 0) {
      if (!state->astro_1->powerup) {
        state->astro_1->powerup = true;
        state->astro_1->powerup_time = 0;
      }
    }
    if (strcmp(PLAYER_TWO, player_num) == 0) {
      if (!state->astro_2->powerup) {
        state->astro_2->powerup = true;
        state->astro_2->powerup_time = 0;
      }
    }
  }
}

void rocketship_handler(body_t *body1, body_t *body2, vector_t axis, void *aux,
                        double force_const) {

  remove_obstacles(body1, (state_t *)aux);
}

void check_removed(state_t *state) {
  for (ssize_t i = list_size(state->obstacles) - 1; i >= 0; i--) {
    if (body_is_removed(scene_get_body(state->scene, i + 2))) {
      // body_free(list_get(state->obstacles, i));
      asset_destroy(list_get(state->obstacle_assets, i));
      list_remove(state->obstacles, i);
      list_remove(state->obstacle_assets, i);
    }
  }
}

void wrap_edges(state_t *state) {
  // start at 2 because first two bodies are players
  for (size_t i = NUM_PLAYERS; i < scene_bodies(state->scene); i++) {
    body_t *body = scene_get_body(state->scene, i);
    vector_t centroid = body_get_centroid(body);
    vector_t velocity = body_get_velocity(body);
    if (centroid.x < MIN.x) {
      bool was_star = false;
      for (size_t j = 0; j < ROWS; j++) {
        if (round(centroid.y) == round(PATHS[j] + STAR_SHIFT)) {
          was_star = true;
          break;
        }
      }
      scene_remove_body(state->scene, i);
      size_t obstacle_index = rand() % NUM_OBSTACLES;

      // meteor --> loses a life if collides with meteor
      if (obstacle_index <= METEOR_INDEX) {
        if (was_star) {
          centroid.y -= STAR_SHIFT;
        }
        body_t *obstacle =
            make_obstacle(OBS_WIDTH, OBS_HEIGHT, (vector_t){MAX.x, centroid.y});

        create_collision(state->scene, state->astro_1->body, obstacle,
                         meteor_handler, state, 0);
        create_collision(state->scene, state->astro_2->body, obstacle,
                         meteor_handler, state, 0);
        list_add(state->obstacles, obstacle);
        asset_t *meteor_img = asset_make_image_with_body(METEOR_PATH, obstacle);
        list_add(state->obstacle_assets, meteor_img);
        scene_add_body(state->scene, obstacle);
        body_set_velocity(obstacle, velocity);
      }

      // shooting_star
      else if (obstacle_index >= STAR_INDEX_1 &&
               obstacle_index <= STAR_INDEX_2) {
        if (!was_star) {
          centroid.y += STAR_SHIFT;
        }
        body_t *star = make_obstacle(STAR_WIDTH, STAR_HEIGHT,
                                     (vector_t){MAX.x, centroid.y});
        body_set_rotation(star, STAR_ROT);
        create_collision(state->scene, state->astro_1->body, star,
                         meteor_handler, state, 0);
        create_collision(state->scene, state->astro_2->body, star,
                         meteor_handler, state, 0);
        list_add(state->obstacles, star);
        asset_t *shooting_star_img =
            asset_make_image_with_body(STAR_PATH, star);
        list_add(state->obstacle_assets, shooting_star_img);
        scene_add_body(state->scene, star);
        body_set_velocity(star, velocity);

      }
      // coin --> increase points by 1
      else if (obstacle_index >= COIN_INDEX_1 &&
               obstacle_index <= COIN_INDEX_2) {
        if (was_star) {
          centroid.y -= STAR_SHIFT;
        }
        body_t *obstacle =
            make_obstacle(OBS_WIDTH, OBS_HEIGHT, (vector_t){MAX.x, centroid.y});
        create_collision(state->scene, state->astro_1->body, obstacle,
                         coin_handler, state, 0);
        create_collision(state->scene, state->astro_2->body, obstacle,
                         coin_handler, state, 0);
        list_add(state->obstacles, obstacle);
        asset_t *coin_img = asset_make_image_with_body(COIN_PATH, obstacle);
        list_add(state->obstacle_assets, coin_img);
        scene_add_body(state->scene, obstacle);
        body_set_velocity(obstacle, velocity);

      }
      // extra coin boost
      else if (obstacle_index >= EXTRA_COIN_1 &&
               obstacle_index <= EXTRA_COIN_2) {
        if (was_star) {
          centroid.y -= STAR_SHIFT;
        }
        body_t *obstacle =
            make_obstacle(OBS_WIDTH, OBS_HEIGHT, (vector_t){MAX.x, centroid.y});
        create_collision(state->scene, state->astro_1->body, obstacle,
                         extra_coin, state, 0);
        create_collision(state->scene, state->astro_2->body, obstacle,
                         extra_coin, state, 0);
        list_add(state->obstacles, obstacle);
        asset_t *extra_coin_img =
            asset_make_image_with_body(EXTRA_COIN_PATH, obstacle);
        list_add(state->obstacle_assets, extra_coin_img);
        scene_add_body(state->scene, obstacle);
        body_set_velocity(obstacle, velocity);
      }
      // rocketship --> goes through all other obstacles without losing a
      // life
      else {
        if (was_star) {
          centroid.y -= STAR_SHIFT;
        }
        body_t *rocket = make_obstacle(STAR_WIDTH, STAR_HEIGHT,
                                       (vector_t){MAX.x, centroid.y});
        body_set_rotation(rocket, ROCKET_ROT);
        create_collision(state->scene, state->astro_1->body, rocket,
                         rocketship_handler, state, 0);
        create_collision(state->scene, state->astro_2->body, rocket,
                         rocketship_handler, state, 0);
        list_add(state->obstacles, rocket);
        asset_t *rocketship_img =
            asset_make_image_with_body(ROCKET_PATH, rocket);
        list_add(state->obstacle_assets, rocketship_img);
        scene_add_body(state->scene, rocket);
        body_set_velocity(rocket, velocity);
      }
    }
  }
}

void on_key(char key, key_event_type_t type, double held_time, state_t *state) {
  body_t *astronaut1 = state->astro_1->body;
  body_t *astronaut2 = state->astro_2->body;
  vector_t translation = (vector_t){0, 0};
  if (type == KEY_PRESSED && type != KEY_RELEASED) {
    switch (key) {
    case LEFT_ARROW:
      if (!state->astro_1->moving_down && !state->astro_1->moving_up) {
        state->astro_1->moving_down = true;
        state->astro_1->move_time = 0;
        state->astro_1->original_position = body_get_centroid(astronaut1);
      }
      break;
    case RIGHT_ARROW:
      if (!state->astro_1->moving_up && !state->astro_1->moving_down) {
        state->astro_1->moving_up = true;
        state->astro_1->move_time = 0;
        state->astro_1->original_position = body_get_centroid(astronaut1);
      }
      break;
    case UP_ARROW:
      translation.y = V_STEP;
      break;
    case DOWN_ARROW:
      translation.y = -V_STEP;
      break;
    case A_BUTTON:
      if (!state->astro_2->moving_down && !state->astro_2->moving_up) {
        state->astro_2->moving_down = true;
        state->astro_2->move_time = 0;
        state->astro_2->original_position = body_get_centroid(astronaut2);
      }
      break;
    case D_BUTTON:
      if (!state->astro_2->moving_up && !state->astro_2->moving_down) {
        state->astro_2->moving_up = true;
        state->astro_2->move_time = 0;
        state->astro_2->original_position = body_get_centroid(astronaut2);
      }
      break;
    case W_BUTTON:
      translation.y = V_STEP;
      break;
    case S_BUTTON:
      translation.y = -V_STEP;
      break;
    }
    if (key == LEFT_ARROW || key == RIGHT_ARROW || key == UP_ARROW ||
        key == DOWN_ARROW) {
      vector_t new_centroid =
          vec_add(body_get_centroid(astronaut1), translation);
      if (new_centroid.x >= MIN.x && new_centroid.x <= MAX.x &&
          new_centroid.y >= MIN.y && new_centroid.y <= MAX.y) {
        body_set_centroid(astronaut1, new_centroid);
      }
    }
    if (key == A_BUTTON || key == D_BUTTON || key == W_BUTTON ||
        key == S_BUTTON) {
      vector_t new_centroid =
          vec_add(body_get_centroid(astronaut2), translation);
      if (new_centroid.x >= MIN.x && new_centroid.x <= MAX.x &&
          new_centroid.y >= MIN.y && new_centroid.y <= MAX.y) {
        body_set_centroid(astronaut2, new_centroid);
      }
    }
  }
}

void update_state(state_t *state, double dt) {
  body_t *astronaut1 = state->astro_1->body;
  body_t *astronaut2 = state->astro_2->body;
  if (state->astro_1->moving_up) {
    state->astro_1->move_time += dt;
    if (state->astro_1->move_time < state->astro_1->move_duration) {
      // Move up
      vector_t new_position =
          vec_add(state->astro_1->original_position, (vector_t){0, H_STEP});
      body_set_centroid(astronaut1, new_position);
    } else {
      // Reset
      body_set_centroid(astronaut1, state->astro_1->original_position);
      state->astro_1->moving_up = false;
    }
  }
  if (state->astro_1->moving_down) {
    state->astro_1->move_time += dt;
    if (state->astro_1->move_time < state->astro_1->move_duration) {
      // Move down
      vector_t new_position =
          vec_add(state->astro_1->original_position, (vector_t){0, -H_STEP});
      body_set_centroid(astronaut1, new_position);
    } else {
      // Reset
      body_set_centroid(astronaut1, state->astro_1->original_position);
      state->astro_1->moving_down = false;
    }
  }
  if (state->astro_2->moving_up) {
    state->astro_2->move_time += dt;
    if (state->astro_2->move_time < state->astro_2->move_duration) {
      // Move up
      vector_t new_position =
          vec_add(state->astro_2->original_position, (vector_t){0, H_STEP});
      body_set_centroid(astronaut2, new_position);
    } else {
      // Reset
      body_set_centroid(astronaut2, state->astro_2->original_position);
      state->astro_2->moving_up = false;
    }
  }
  if (state->astro_2->moving_down) {
    state->astro_2->move_time += dt;
    if (state->astro_2->move_time < state->astro_2->move_duration) {
      // Move down
      vector_t new_position =
          vec_add(state->astro_2->original_position, (vector_t){0, -H_STEP});
      body_set_centroid(astronaut2, new_position);
    } else {
      // Reset
      body_set_centroid(astronaut2, state->astro_2->original_position);
      state->astro_2->moving_down = false;
    }
  }
  for (size_t i = 0; i < list_size(state->obstacles); i++) {
    body_t *obstacle = list_get(state->obstacles, i);
    vector_t current_velocity = body_get_velocity(obstacle);
    vector_t increased_velocity = vec_subtract(
        current_velocity, (vector_t){dt * INCREASED_VELOCITY_FACTOR, 0});
    body_set_velocity(obstacle, increased_velocity);
  }
  state->astro_1->move_duration /= DECREASED_JUMP_FACTOR;
  state->astro_2->move_duration /= DECREASED_JUMP_FACTOR;
}

double rand_double(double low, double high) {
  return (high - low) * rand() / RAND_MAX + low;
}

void render_start_screen(state_t *state) {
  body_t *player1 =
      make_obstacle(CHARACTER_SIZE1, CHARACTER_SIZE1, PLAYER_1_POS_INITIAL);
  asset_render(state->single_player_button);
  asset_render(state->dual_player_button);
  asset_t *start_message1 = asset_make_text(FONT1, (SDL_Rect){40, 25, 500, 300},
                                            START_MESSAGE1, WHITE, FONT_SIZE1);
  asset_render(start_message1);
  asset_t *start_message2 = asset_make_text(FONT, (SDL_Rect){40, 100, 300, 50},
                                            START_MESSAGE2, WHITE, FONT_SIZE2);
  asset_render(start_message2);
  asset_t *start_message3 = asset_make_text(FONT, (SDL_Rect){40, 120, 300, 50},
                                            START_MESSAGE3, WHITE, FONT_SIZE2);
  asset_render(start_message3);
  asset_t *player1_char =
      asset_make_image_with_body(CHARACTER_PATHS[0], player1);
  asset_render(player1_char);
  asset_destroy(start_message1);
  asset_destroy(start_message2);
  asset_destroy(start_message3);
  asset_destroy(player1_char);
}

void render_single_custom(state_t *state) {
  sdl_clear();
  asset_render(list_get(state->backgrounds, state->background_index));

  list_t *buttons = state->manual_buttons;
  for (size_t i = 0; i < list_size(buttons); i++) {
    asset_render(list_get(buttons, i));
    float center_y = button_templates[i + 3].image_box.y - 50;
    float center_x = 0;
    if (i > 2) {
      center_x = button_templates[i + 3].image_box.x - 50;
    } else {
      center_x = button_templates[i + 3].image_box.x + 250;
    }
    body_t *player = make_obstacle(CHARACTER_SIZE, CHARACTER_SIZE,
                                   (vector_t){center_x, center_y});
    asset_render(asset_make_image_with_body(CHARACTER_PATHS[i], player));
  }
}

void render_in_game(state_t *state, double dt) {
  asset_render(list_get(state->backgrounds, state->background_index));
  for (size_t i = 0; i < list_size(state->obstacle_assets); i++) {
    asset_render(list_get(state->obstacle_assets, i));
  }
  for (size_t j = 0; j < list_size(state->player_assets); j++) {
    asset_render(list_get(state->player_assets, j));
  }
  for (size_t i = 0; i < state->astro_1->health; i++) {
    body_t *health = make_obstacle(20, 20, (vector_t)positions_health[i]);
    asset_t *heart = asset_make_image_with_body(HEART_PATH, health);
    asset_render(heart);
    asset_destroy(heart);
  }
  if (state->astro_1->powerup) {
    state->astro_1->powerup_time += dt;
    char *powerup_message1 = malloc(sizeof(char) * NUM_CHARACTERS);
    assert(powerup_message1 != NULL);
    sprintf(powerup_message1, "Player 1 has a power up for: %d",
            (int)(POWERUP_INTERVAL - state->astro_1->powerup_time));
    asset_t *powerup_text1 =
        asset_make_text(FONT, (SDL_Rect){200, 340, 100, 15}, powerup_message1,
                        WHITE, FONT_SIZE2);
    asset_render(powerup_text1);
    if (state->astro_1->powerup_time >= POWERUP_INTERVAL) {
      state->astro_1->powerup = false;
    }
    free(powerup_message1);
    asset_destroy(powerup_text1);
  }
  if (state->astro_2->powerup) {
    state->astro_2->powerup_time += dt;
    char *powerup_message2 = malloc(sizeof(char) * NUM_CHARACTERS);
    assert(powerup_message2 != NULL);
    sprintf(powerup_message2, "Player 2 has a power up for: %d",
            (int)(POWERUP_INTERVAL - state->astro_2->powerup_time));
    asset_t *powerup_text2 =
        asset_make_text(FONT, (SDL_Rect){200, 375, 100, 15}, powerup_message2,
                        WHITE, FONT_SIZE2);
    asset_render(powerup_text2);
    if (state->astro_2->powerup_time >= POWERUP_INTERVAL) {
      state->astro_2->powerup = false;
    }
    free(powerup_message2);
    asset_destroy(powerup_text2);
  }
  if (state->astro_1->increase_health) {
    state->astro_1->coin_time += dt;
    asset_t *health_text1 = asset_make_text(FONT, (SDL_Rect){100, 225, 390, 15},
                                            LIFE_MESSAGE1, WHITE, FONT_SIZE2);
    asset_render(health_text1);
    if (state->astro_1->coin_time > 3) {
      state->astro_1->increase_health = false;
    }
    asset_destroy(health_text1);
  }
  if (state->astro_2->increase_health) {
    state->astro_2->coin_time += dt;
    asset_t *health_text2 = asset_make_text(FONT, (SDL_Rect){300, 225, 390, 15},
                                            LIFE_MESSAGE2, WHITE, FONT_SIZE2);
    asset_render(health_text2);
    if (state->astro_2->coin_time > 3) {
      state->astro_2->increase_health = false;
    }
    asset_destroy(health_text2);
  }
  asset_t *player1_text = asset_make_text(FONT, (SDL_Rect){390, 15, 390, 15},
                                          PLAYER1_MESSAGE, WHITE, FONT_SIZE2);
  asset_render(player1_text);
  update_state(state, dt);
  asset_destroy(player1_text);
}

state_t *emscripten_init() {
  asset_cache_init();
  sdl_init(MIN, MAX);
  state_t *state = malloc(sizeof(state_t));
  assert(state != NULL);
  state->astro_1 = astronaut_init();
  state->astro_2 = astronaut_init();
  srand(time(NULL));
  state->scene = scene_init();
  state->obstacle_assets = list_init(2, (free_func_t)asset_destroy);
  state->player_assets = list_init(2, (free_func_t)asset_destroy);
  state->backgrounds = list_init(2, (free_func_t)asset_destroy);
  state->obstacles = list_init(2, (free_func_t)body_free);
  state->background_index = 0;
  state->time = 0;
  state->sound = false;

  body_t *astro_body =
      make_astro(OUTER_RADIUS, INNER_RADIUS, VEC_ZERO, PLAYER_ONE);
  state->astro_1->body = astro_body;
  body_set_centroid(astro_body, RESET_POS);
  scene_add_body(state->scene, astro_body);
  asset_t *astro_img =
      asset_make_image_with_body(CHARACTER_PATHS[0], astro_body);
  state->astro_1->asset = astro_img;
  list_add(state->player_assets, astro_img);

  body_t *astro_body_2 =
      make_astro(OUTER_RADIUS, INNER_RADIUS, VEC_ZERO, PLAYER_TWO);
  state->astro_2->body = astro_body_2;
  asset_t *astro_2_img =
      asset_make_image_with_body(CHARACTER_PATHS[1], state->astro_2->body);
  state->astro_2->asset = astro_2_img;
  body_set_centroid(astro_body_2, RESET_POS_2);
  scene_add_body(state->scene, astro_body_2);
  list_add(state->player_assets, astro_2_img);

  SDL_Rect start = {
      .x = MIN.x, .y = MIN.y, .w = MAX.x - MIN.x, .h = MAX.y - MIN.y};

  for (size_t i = 0; i < NUM_BACKGROUND; i++) {
    asset_t *start_back = asset_make_image(BACKGROUNDS[i], start);
    list_add(state->backgrounds, start_back);
  }
  state->manual_buttons = list_init(NUM_BUTTONS, NULL);
  state->button_assets = list_init(NUM_BUTTONS, (free_func_t)asset_destroy);
  create_buttons(state);
  for (size_t i = 0; i < ROWS; i++) {
    double cx = 0;
    double cy = PATHS[i];
    while (cx < MAX.x) {
      body_t *obstacle =
          make_obstacle(OBS_WIDTH, OBS_HEIGHT, (vector_t){cx, cy});
      size_t obstacle_index = rand() % NUM_OBSTACLES;

      // meteor --> loses a life if collides with meteor
      if (obstacle_index <= METEOR_INDEX) {
        create_collision(state->scene, astro_body, obstacle, meteor_handler,
                         state, 0);
        create_collision(state->scene, astro_body_2, obstacle, meteor_handler,
                         state, 0);
        list_add(state->obstacles, obstacle);
        asset_t *log_img = asset_make_image_with_body(METEOR_PATH, obstacle);
        list_add(state->obstacle_assets, log_img);
        scene_add_body(state->scene, obstacle);
        cx += OBS_WIDTH + rand_double(OBS_SPACING.x, OBS_SPACING.y);
      }

      // shooting star
      else if (obstacle_index >= STAR_INDEX_1 &&
               obstacle_index <= STAR_INDEX_2) {
        body_t *star = make_obstacle(STAR_WIDTH, STAR_HEIGHT,
                                     (vector_t){cx, cy + STAR_SHIFT});
        body_set_rotation(star, STAR_ROT);
        cx += STAR_ROCKET_SHIFT + rand_double(OBS_SPACING.x, OBS_SPACING.y);
        cx += STAR_ROCKET_SHIFT + rand_double(OBS_SPACING.x, OBS_SPACING.y);
        create_collision(state->scene, astro_body, star, meteor_handler, state,
                         0);
        create_collision(state->scene, astro_body_2, star, meteor_handler,
                         state, 0);
        list_add(state->obstacles, star);
        asset_t *log_img = asset_make_image_with_body(STAR_PATH, star);
        list_add(state->obstacle_assets, log_img);
        scene_add_body(state->scene, star);

      }
      // coin --> increase points by 1
      else if (obstacle_index >= COIN_INDEX_1 &&
               obstacle_index <= COIN_INDEX_2) {
        create_collision(state->scene, astro_body, obstacle, coin_handler,
                         state, 0);
        create_collision(state->scene, astro_body_2, obstacle, coin_handler,
                         state, 0);
        list_add(state->obstacles, obstacle);
        asset_t *log_img = asset_make_image_with_body(COIN_PATH, obstacle);
        list_add(state->obstacle_assets, log_img);
        scene_add_body(state->scene, obstacle);
        cx += OBS_WIDTH + rand_double(OBS_SPACING.x, OBS_SPACING.y);
      }
      // extra coin boost
      else if (obstacle_index >= EXTRA_COIN_1 &&
               obstacle_index <= EXTRA_COIN_2) {
        create_collision(state->scene, astro_body, obstacle, extra_coin, state,
                         0);
        create_collision(state->scene, astro_body_2, obstacle, extra_coin,
                         state, 0);
        list_add(state->obstacles, obstacle);
        asset_t *log_img =
            asset_make_image_with_body(EXTRA_COIN_PATH, obstacle);
        list_add(state->obstacle_assets, log_img);
        scene_add_body(state->scene, obstacle);
        cx += OBS_WIDTH + rand_double(OBS_SPACING.x, OBS_SPACING.y);
      }
      // rocketship --> goes through all other obstacles without losing a life
      else {
        body_t *rocket =
            make_obstacle(STAR_WIDTH, STAR_HEIGHT, (vector_t){cx, cy});
        body_set_rotation(rocket, ROCKET_ROT);
        create_collision(state->scene, astro_body, rocket, rocketship_handler,
                         state, 0);
        create_collision(state->scene, astro_body_2, rocket, rocketship_handler,
                         state, 0);
        list_add(state->obstacles, rocket);
        asset_t *log_img = asset_make_image_with_body(ROCKET_PATH, rocket);
        list_add(state->obstacle_assets, log_img);
        scene_add_body(state->scene, rocket);
        cx += STAR_ROCKET_SHIFT + rand_double(OBS_SPACING.x, OBS_SPACING.y);
      }
    }
  }
  sdl_on_key((key_handler_t)on_key);
  return state;
}

bool emscripten_main(state_t *state) {
  double dt = time_since_last_tick();
  state->time += dt;
  if ((state->background_index == NUM_BACKGROUND - 1) && state->single) {
    sdl_clear();
    asset_render(list_get(state->backgrounds, state->background_index));
    sdl_show();
    if (!state->sound) {
      load_sound(CHANNEL, GAME_OVER_SOUND);
      state->sound = true;
      for (size_t i = 0; i < list_size(state->player_assets); i++) {
        asset_destroy((asset_t *)list_get(state->player_assets, i));
      }
      for (size_t i = 0; i < list_size(state->obstacle_assets); i++) {
        asset_destroy((asset_t *)list_get(state->obstacle_assets, i));
      }
      scene_free(state->scene);
    }
    return false;
  }

  if ((state->background_index == NUM_BACKGROUND - 1) && state->dual) {
    sdl_clear();
    asset_render(list_get(state->backgrounds, state->background_index));
    if (state->astro_1->health > state->astro_2->health) {
      asset_t *winner_text1 =
          asset_make_text(FONT, (SDL_Rect){200, 100, 200, 100}, WINNER_MESSAGE1,
                          WHITE, FONT_SIZE2);
      asset_render(winner_text1);
      asset_destroy(winner_text1);
    } else if (state->astro_1->health < state->astro_2->health) {
      asset_t *winner_text2 =
          asset_make_text(FONT, (SDL_Rect){200, 100, 200, 100}, WINNER_MESSAGE2,
                          WHITE, FONT_SIZE2);
      asset_render(winner_text2);
      asset_destroy(winner_text2);
    } else {
      asset_t *winner_text3 =
          asset_make_text(FONT, (SDL_Rect){200, 100, 200, 100},
                          WINNER_TIE_MESSAGE, WHITE, FONT_SIZE2);
      asset_render(winner_text3);
      asset_destroy(winner_text3);
    }
    sdl_show();
    if (!state->sound) {
      load_sound(CHANNEL, GAME_OVER_SOUND);
      state->sound = true;
      for (size_t i = 0; i < list_size(state->player_assets); i++) {
        asset_destroy((asset_t *)list_get(state->player_assets, i));
      }
      for (size_t i = 0; i < list_size(state->obstacle_assets); i++) {
        asset_destroy((asset_t *)list_get(state->obstacle_assets, i));
      }
      scene_free(state->scene);
    }
    return false;
  }

  if (state->time > INTERVAL && state->background_index >= CHANGE_INDEX) {
    state->background_index++;
    state->time -= INTERVAL;
  }
  asset_render(list_get(state->backgrounds, state->background_index));
  render_start_screen(state);
  if (state->single) {
    render_single_custom(state);
    asset_t *choose_text =
        asset_make_text(FONT1, (SDL_Rect){100, 10, 100, 30},
                        CHOOSE_TEXT_MESSAGE, WHITE, FONT_SIZE2);
    asset_render(choose_text);
    asset_t *instructions_text =
        asset_make_text(FONT, (SDL_Rect){140, 70, 100, 30},
                        INSTRUCTIONS_MESSAGE1, WHITE, FONT_SIZE2);
    asset_render(instructions_text);
    if (state->is_customized) {
      wrap_edges(state);
      check_removed(state);
      render_in_game(state, dt);
      char *coin_text1 = malloc(sizeof(char) * NUM_CHARACTERS);
      sprintf(coin_text1, "Coins: %d", (int)state->astro_1->points);
      asset_t *coin_text = asset_make_text(FONT, (SDL_Rect){10, 15, 390, 15},
                                           coin_text1, WHITE, FONT_SIZE2);
      asset_render(coin_text);
      free(coin_text1);
      asset_destroy(coin_text);
    }
    asset_destroy(choose_text);
    asset_destroy(instructions_text);
  }

  if (state->dual) {
    sdl_clear();
    asset_render(list_get(state->backgrounds, state->background_index));
    asset_render(state->start_button);
    body_t *player1 =
        make_obstacle(CHARACTER_SIZE, CHARACTER_SIZE, PLAYER_1_POS);
    asset_render(asset_make_image_with_body(CHARACTER_PATHS[0], player1));
    body_t *player2 =
        make_obstacle(CHARACTER_SIZE, CHARACTER_SIZE, PLAYER_2_POS);
    asset_render(asset_make_image_with_body(CHARACTER_PATHS[1], player2));
    asset_t *instructions_text1 =
        asset_make_text(FONT, (SDL_Rect){25, 100, 100, 100},
                        INSTRUCTIONS_MESSAGE1, WHITE, FONT_SIZE1);
    asset_render(instructions_text1);
    asset_t *instructions_text2 =
        asset_make_text(FONT, (SDL_Rect){25, 300, 100, 100},
                        INSTRUCTIONS_MESSAGE2, WHITE, FONT_SIZE2);
    asset_render(instructions_text2);
    if (state->is_customized) {
      wrap_edges(state);
      check_removed(state);
      render_in_game(state, dt);
      for (size_t i = 0; i < state->astro_2->health; i++) {
        body_t *health = make_obstacle(20, 20, (vector_t)positions_health_2[i]);
        asset_t *heart = asset_make_image_with_body(HEART_PATH, health);
        asset_render(heart);
        asset_destroy(heart);
      }
      asset_t *player2_text =
          asset_make_text(FONT, (SDL_Rect){145, 15, 145, 15}, PLAYER2_MESSAGE,
                          WHITE, FONT_SIZE2);
      asset_render(player2_text);
      char *coin_text2 = malloc(sizeof(char) * NUM_CHARACTERS);
      sprintf(coin_text2, "Coins: %d", (int)state->astro_1->points);
      asset_t *coin_message2 = asset_make_text(
          FONT, (SDL_Rect){500, 350, 390, 15}, coin_text2, WHITE, FONT_SIZE2);
      asset_render(coin_message2);
      char *coin_text3 = malloc(sizeof(char) * NUM_CHARACTERS);
      sprintf(coin_text3, "Coins: %d", (int)state->astro_2->points);
      asset_t *coin_message3 = asset_make_text(
          FONT, (SDL_Rect){100, 350, 390, 15}, coin_text3, WHITE, FONT_SIZE2);
      asset_render(coin_message3);
      asset_destroy(player2_text);
      free(coin_text2);
      asset_destroy(coin_message2);
      free(coin_text3);
      asset_destroy(coin_message3);
    }
    asset_destroy(instructions_text1);
    asset_destroy(instructions_text2);
  }

  sdl_show();
  scene_tick(state->scene, dt);
  return false;
}

void emscripten_free(state_t *state) {
  printf("Emscripten free was called");
  list_free(state->obstacle_assets);
  list_free(state->player_assets);
  astronaut_free(state->astro_1);
  astronaut_free(state->astro_2);
  list_free(state->obstacles);
  scene_free(state->scene);
  asset_cache_destroy();
  list_free(state->backgrounds);
  list_free(state->button_assets);
  list_free(state->manual_buttons);
  asset_destroy(state->single_player_button);
  asset_destroy(state->dual_player_button);
  asset_destroy(state->start_button);
  free(state);
  free_sound();
}
