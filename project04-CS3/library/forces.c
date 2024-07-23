#include "forces.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "list.h"

const double MIN_DIST = 5;

typedef struct {
  double force_const;
  list_t *bodies;
} body_aux_t;

body_aux_t *body_aux_init(double force_const, list_t *bodies) {
  body_aux_t *aux = malloc(sizeof(body_aux_t));
  assert(aux);
  aux->bodies = bodies;
  aux->force_const = force_const;
  return aux;
}

void body_aux_free(void *aux) { 
  list_free(((body_aux_t *)aux)->bodies);
  free(aux); 
}

void newtonian_gravity(void *info) {
  body_aux_t *aux = (body_aux_t *)info;
  vector_t displacement = vec_subtract(body_get_centroid(list_get(aux->bodies, 0)),
                                       body_get_centroid(list_get(aux->bodies, 1)));
  vector_t unit_disp =
      vec_multiply(1 / sqrt(vec_dot(displacement, displacement)), displacement);

  double distance = sqrt(vec_dot(displacement, displacement));

  if (distance > MIN_DIST) {
    vector_t grav_force = vec_multiply(
        aux->force_const * body_get_mass(list_get(aux->bodies, 0)) *
            body_get_mass(list_get(aux->bodies, 1)) / vec_dot(displacement, displacement),
        unit_disp);

    body_add_force(list_get(aux->bodies, 1), grav_force);
    body_add_force(list_get(aux->bodies, 0), vec_multiply(-1, grav_force));
  }
}

void create_newtonian_gravity(scene_t *scene, double G, body_t *body1,
                              body_t *body2) {
  list_t *bodies = list_init(2, NULL);
  list_add(bodies, body1);
  list_add(bodies, body2);
  body_aux_t *aux = body_aux_init(G, bodies);
  scene_add_force_creator(scene, (force_creator_t)newtonian_gravity, aux);
}

void spring_force(void *info) {
  body_aux_t *aux = info;

  double k = aux->force_const;
  body_t *body1 = list_get(aux->bodies, 0);
  body_t *body2 = list_get(aux->bodies, 1);

  vector_t center_1 = body_get_centroid(body1);
  vector_t center_2 = body_get_centroid(body2);
  vector_t distance = vec_subtract(center_1, center_2);

  vector_t spring_force = {-k * distance.x, -k * distance.y};
  body_add_force(body1, spring_force);
  body_add_force(body2, vec_negate(spring_force));
}

void create_spring(scene_t *scene, double k, body_t *body1, body_t *body2) {
  list_t *bodies = list_init(2, NULL);
  list_add(bodies, body1);
  list_add(bodies, body2);
  body_aux_t *aux = body_aux_init(k, bodies);
  scene_add_force_creator(scene, (force_creator_t)spring_force, aux);
}

void drag_force(void *info) {
  body_aux_t *aux = (body_aux_t *)info;
  vector_t cons_force =
      vec_multiply(-1 * aux->force_const, body_get_velocity(list_get(aux->bodies, 0)));

  body_add_force(list_get(aux->bodies, 0), cons_force);
}

void create_drag(scene_t *scene, double gamma, body_t *body) {
  list_t *bodies = list_init(1, NULL);
  list_add(bodies, body);
  body_aux_t *aux = body_aux_init(gamma, bodies);
  scene_add_force_creator(scene, (force_creator_t)drag_force, aux);
}
