
#include "vector.h"
#include <math.h>

const vector_t VEC_ZERO = {0, 0};

double vec_get_length(vector_t v) { return sqrt(pow(v.x, 2) + pow(v.y, 2)); }

vector_t vec_add(vector_t v1, vector_t v2) {
  vector_t addedVector = {v1.x + v2.x, v1.y + v2.y};
  return addedVector;
}

vector_t vec_subtract(vector_t v1, vector_t v2) {
  vector_t subtractedVector = {v1.x - v2.x, v1.y - v2.y};
  return subtractedVector;
}

vector_t vec_negate(vector_t v) {
  vector_t negativeVector = {-v.x, -v.y};
  return negativeVector;
}

vector_t vec_multiply(double scalar, vector_t v) {
  vector_t scalarVector = {scalar * v.x, scalar * v.y};
  return scalarVector;
}

double vec_dot(vector_t v1, vector_t v2) {
  double dotProduct = (v1.x * v2.x) + (v1.y * v2.y);
  return dotProduct;
}

double vec_cross(vector_t v1, vector_t v2) {
  double cross = (v1.x * v2.y) - (v2.x * v1.y);
  return cross;
}

vector_t vec_rotate(vector_t v, double angle) {
  vector_t roate = {v.x * cos(angle) - v.y * sin(angle),
                    v.x * sin(angle) + v.y * cos(angle)};
  return roate;
}
