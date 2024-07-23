#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "scene.h"
#include "forces.h"




const size_t I_SIZE = 5;
/* Feel free to add to this struct to incorporate forces 


A collection of bodies and force creators.
 * The scene automatically resizes to store
 * arbitrarily many bodies and force creators.


*/
struct scene {
  list_t *bodies;
  list_t *forces;
  list_t *auxes;

};


scene_t *scene_init(void) {
  scene_t *scene = malloc(sizeof(scene_t));
  assert(scene != NULL);
  scene->bodies = list_init(1, (free_func_t)body_free);
  scene->forces = list_init(I_SIZE, (free_func_t)NULL);
  scene->auxes = list_init(I_SIZE,(free_func_t)body_aux_free); 
  return scene;
}


void scene_free(scene_t *scene) {
  list_free(scene->forces);
  list_free(scene->bodies);
  list_free(scene->auxes);
  free(scene);
}


size_t scene_bodies(scene_t *scene) { return list_size(scene->bodies); }

body_t *scene_get_body(scene_t *scene, size_t index) {
  assert(index < list_size(scene->bodies));
  return list_get(scene->bodies, index);
}

void scene_add_body(scene_t *scene, body_t *body) {
  list_add(scene->bodies, body);
}


void scene_remove_body(scene_t *scene, size_t index) {
  assert(index < list_size(scene->bodies));
  list_remove(scene->bodies, index);
}


void scene_add_force_creator(scene_t *scene, force_creator_t force_creator, void *aux) {
  list_add(scene->forces, force_creator);
  list_add(scene->auxes, aux);
}

void scene_tick(scene_t *scene, double dt) {
  size_t size = list_size(scene->forces);
  for(size_t i = 0; i < size; i++) {
    force_creator_t function = list_get(scene->forces, i);
    function(list_get(scene->auxes, i));
  }
  for (size_t i = 0; i < list_size(scene->bodies); i++) {
    body_t *body = scene_get_body(scene, i);
    body_tick(body, dt);
  }
}





