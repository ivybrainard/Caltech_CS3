#include "scene.h"
#include "test_util.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>

void scene_get_first(void *scene) { scene_get_body(scene, 0); }
void scene_remove_first(void *scene) { scene_remove_body(scene, 0); }

void test_empty_scene() {
  scene_t *scene = scene_init();
  assert(scene_bodies(scene) == 0);
  for (int i = 0; i < 10; i++)
    scene_tick(scene, 1);
  assert(test_assert_fail(scene_get_first, scene));
  assert(test_assert_fail(scene_remove_first, scene));
  scene_free(scene);
}

list_t *make_shape() {
  list_t *shape = list_init(4, free);
  vector_t *v = malloc(sizeof(*v));
  *v = (vector_t){-1, -1};
  list_add(shape, v);
  v = malloc(sizeof(*v));
  *v = (vector_t){+1, -1};
  list_add(shape, v);
  v = malloc(sizeof(*v));
  *v = (vector_t){+1, +1};
  list_add(shape, v);
  v = malloc(sizeof(*v));
  *v = (vector_t){-1, +1};
  list_add(shape, v);
  return shape;
}

void test_scene() {
  // Build a scene with 3 bodies
  scene_t *scene = scene_init();
  assert(scene_bodies(scene) == 0);
  body_t *body1 = body_init(make_shape(), 1, (rgb_color_t){1, 1, 1});
  scene_add_body(scene, body1);
  assert(scene_bodies(scene) == 1);
  assert(scene_get_body(scene, 0) == body1);
  body_t *body2 = body_init(make_shape(), 2, (rgb_color_t){1, 1, 1});
  scene_add_body(scene, body2);
  assert(scene_bodies(scene) == 2);
  assert(scene_get_body(scene, 0) == body1);
  assert(scene_get_body(scene, 1) == body2);
  body_t *body3 = body_init(make_shape(), 3, (rgb_color_t){1, 1, 1});
  scene_add_body(scene, body3);
  assert(scene_bodies(scene) == 3);
  assert(scene_get_body(scene, 0) == body1);
  assert(scene_get_body(scene, 1) == body2);
  assert(scene_get_body(scene, 2) == body3);

  // Set the bodies' positions with no velocity and ensure they match
  body_set_centroid(body1, (vector_t){1, 1});
  body_set_centroid(body2, (vector_t){2, 2});
  body_set_centroid(body3, (vector_t){3, 3});
  scene_tick(scene, 1);
  assert(vec_isclose(body_get_centroid(body1), (vector_t){1, 1}));
  assert(vec_isclose(body_get_centroid(body2), (vector_t){2, 2}));
  assert(vec_isclose(body_get_centroid(body3), (vector_t){3, 3}));
  body_set_velocity(body1, (vector_t){+1, 0});
  body_set_velocity(body2, (vector_t){-1, 0});
  body_set_velocity(body3, (vector_t){0, +1});
  scene_tick(scene, 1);
  assert(vec_isclose(body_get_centroid(body1), (vector_t){2, 1}));
  assert(vec_isclose(body_get_centroid(body2), (vector_t){1, 2}));
  assert(vec_isclose(body_get_centroid(body3), (vector_t){3, 4}));

  // Try removing the second body
  assert(scene_bodies(scene) == 3);
  scene_remove_body(scene, 1);
  body_free(body2);
  scene_tick(scene, 0);
  assert(scene_bodies(scene) == 2);
  assert(scene_get_body(scene, 0) == body1);
  assert(scene_get_body(scene, 1) == body3);

  // Tick the remaining bodies
  scene_tick(scene, 1);
  assert(vec_isclose(body_get_centroid(body1), (vector_t){3, 1}));
  assert(vec_isclose(body_get_centroid(body3), (vector_t){3, 5}));

  scene_free(scene);
}

int main(int argc, char *argv[]) {
  // Run all tests if there are no command-line arguments
  bool all_tests = argc == 1;
  // Read test name from file
  char testname[100];
  if (!all_tests) {
    read_testname(argv[1], testname, sizeof(testname));
  }

  DO_TEST(test_empty_scene)
  DO_TEST(test_scene)

  puts("scene_test PASS");
}
