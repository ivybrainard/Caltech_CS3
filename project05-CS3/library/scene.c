#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "forces.h"
#include "list.h"
#include "scene.h"

const size_t I_SIZE = 5;

struct scene {
  size_t num_bodies;
  list_t *bodies;
  list_t *forces;
};

void scene_tick(scene_t *scene, double dt) {
  size_t size = list_size(scene->forces);
  for (ssize_t i = 0; i < (ssize_t)size; i++) {
    force_t *f = list_get(scene->forces, i);
    force_creator_t function = f->force_func;
    function(f->aux);
  }

  size_t num = list_size(scene->bodies);
  for (ssize_t i = 0; i < (ssize_t)num; i++) {
    body_t *curr_body = scene_get_body(scene, i);
    if (body_is_removed(curr_body)) {
      size_t force_length = list_size(scene->forces);
      for (ssize_t j = 0; j < (ssize_t)force_length; j++) {
        force_t *curr_force = list_get(scene->forces, j);
        list_t *curr_force_bodies = curr_force->bodies;
        size_t bod_list_length = list_size(curr_force_bodies);
        for (ssize_t k = 0; k < (ssize_t)bod_list_length; k++) {
          if (list_get(curr_force_bodies, k) == curr_body) {
            force_free(list_remove(scene->forces, j));
            force_length--;
            j--;
            break;
          }
        }
      }
      body_free(list_remove(scene->bodies, i));
      num--;
      i--;
    } else {
      body_tick(curr_body, dt);
    }
  }
}

void scene_add_force_creator(scene_t *scene, force_creator_t force_creator,
                             void *aux) {
  scene_add_bodies_force_creator(scene, force_creator, aux, list_init(0, free));
}

void scene_add_bodies_force_creator(scene_t *scene, force_creator_t forcer,
                                    void *aux, list_t *bodies) {
  force_t *force = force_init(forcer, aux, bodies);
  list_add(scene->forces, force);
}

scene_t *scene_init(void) {
  scene_t *scene = malloc(sizeof(scene_t));
  assert(scene != NULL);
  scene->bodies = list_init(1, (free_func_t)body_free);
  scene->forces = list_init(1, (free_func_t)force_free);
  return scene;
}

void scene_free(scene_t *scene) {
  list_free(scene->bodies);
  list_free(scene->forces);
  free(scene);
}

size_t scene_bodies(scene_t *scene) { return list_size(scene->bodies); }

body_t *scene_get_body(scene_t *scene, size_t index) {
  assert(index < list_size(scene->bodies));
  assert(index >= 0);
  return list_get(scene->bodies, index);
}

void scene_add_body(scene_t *scene, body_t *body) {
  list_add(scene->bodies, body);
  scene->num_bodies++;
}

void scene_remove_body(scene_t *scene, size_t index) {
  assert(index < list_size(scene->bodies));
  assert(index >= 0);
  body_remove(list_get(scene->bodies, index));
}
