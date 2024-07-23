#include "test_util.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "color.h"

void test_color_init() {
  rgb_color_t *color = color_init(0, 0, 0);
  assert(color);
  color_free(color);
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

  puts("color_test PASS");
}
