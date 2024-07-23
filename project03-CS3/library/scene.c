#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "scene.h"

struct scene {
  size_t num_bodies;
  list_t *bodies;
};

scene_t *scene_init(void) {
  scene_t *scene = malloc(sizeof(scene_t));
  assert(scene != NULL);
  scene->num_bodies = 0;
  scene->bodies = list_init(1, (free_func_t)body_free);
  return scene;
}

void scene_free(scene_t *scene) {
  list_free(scene->bodies);
  free(scene);
}

size_t scene_bodies(scene_t *scene) { return scene->num_bodies; }

body_t *scene_get_body(scene_t *scene, size_t index) {
  assert(index < scene->num_bodies);
  return list_get(scene->bodies, index);
}

void scene_add_body(scene_t *scene, body_t *body) {
  list_add(scene->bodies, body);
  scene->num_bodies++;
}

void scene_remove_body(scene_t *scene, size_t index) {
  assert(index < scene->num_bodies);
  list_remove(scene->bodies, index);
  scene->num_bodies--;
}

void scene_tick(scene_t *scene, double dt) {
  for (size_t i = 0; i < scene->num_bodies; i++) {
    body_t *body = scene_get_body(scene, i);
    body_tick(body, dt);
  }
}