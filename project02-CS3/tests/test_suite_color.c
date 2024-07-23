#include "test_util.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "color.h"

void test_color_init() {
  color_t *color = color_init(0, 0, 0);
  assert(color);
  color_free(color);
}

void test_color_values() {
  double r = 0.2;
  double b = 0.4;
  double g = 0.6;

  color_t *color2 = color_init(r, g, b);
  assert(color_get_red(color2) == r);
  assert(color_get_green(color2) == g);
  assert(color_get_blue(color2) == b);

  color_free(color2);
}

int main(int argc, char *argv[]) {
  // Run all tests if there are no command-line arguments
  bool all_tests = argc == 1;
  // Read test name from file
  char testname[100];
  if (!all_tests) {
    read_testname(argv[1], testname, sizeof(testname));
  }

  DO_TEST(test_color_init)
  DO_TEST(test_color_values)

  puts("color_test PASS");
}
