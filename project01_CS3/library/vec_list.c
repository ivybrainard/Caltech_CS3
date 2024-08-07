#include "vec_list.h"
#include <assert.h>
#include <stdlib.h>

/**
 * A growable array of vectors, stored as pointers to malloc()ed vectors.
 * A list owns all the vectors in it, so it is responsible for free()ing them.
 * This line does two things:
 * - Declares a "struct vec_list" type
 * - Makes "vec_list_t" an alias for "struct vec_list"
 *
 * You will need to implement this struct type in vec_list.c.
 */

// typedef struct vec_list {
//     vector_t **arrayVector;
//     size_t length;
//     size_t capacity;
// } vec_list_t;

/**
 * Allocates memory for a new list with space for the given number of elements.
 * The list is initially empty.
 * Asserts that the required memory was allocated.
 *
 * @param initial_size the number of list elements to allocate space for
 * @return a pointer to the newly allocated list
 */
vec_list_t *vec_list_init(size_t initial_size) {
  vec_list_t *array = malloc(sizeof(vec_list_t));
  assert(array);
  array->arrayVector = calloc(initial_size, sizeof(vector_t *));
  array->length = 0;
  array->capacity = initial_size;
  return array;
}

/**
 * Releases the memory allocated for a list.
 * Also frees all vectors in the list.
 *
 * @param list a pointer to a list returned from vec_list_init()
 */
void vec_list_free(vec_list_t *list) { // added static
  for (size_t i = 0; i < list->length; i++) {
    free(list->arrayVector[i]);
  }
  free(list->arrayVector);
  free(list);
}

/**
 * Gets the size of a list (the number of occupied elements).
 * Note that this is NOT the list's capacity.
 *
 * @param list a pointer to a list returned from vec_list_init()
 * @return the number of vectors in the list
 */
size_t vec_list_size(vec_list_t *list) {
  size_t size = list->length;
  return size;
}

/**
 * Gets the element at a given index in a list.
 * Asserts that the index is valid, given the list's current size.
 *
 * @param list a pointer to a list returned from vec_list_init()
 * @param index an index in the list (the first element is at 0)
 * @return the vector at the given index
 */
vector_t *vec_list_get(vec_list_t *list, size_t index) {
  assert(index < list->length);
  assert(index >= 0);
  vector_t *chosenVector = list->arrayVector[index];
  return chosenVector;
}

/**
 * Appends an element to the end of a list.
 * Asserts that the list has has remaining space,
 * and that the value being added is not NULL.
 *
 * @param list a pointer to a list returned from vec_list_init()
 * @param value the vector to add to the end of the list
 */
void vec_list_add(vec_list_t *list, vector_t *value) {
  assert(list->length < list->capacity);
  assert(value != NULL);
  list->arrayVector[list->length] = value;
  list->length++;
}

/**
 * Removes the element at the end of a list and returns it.
 * Asserts that the list is nonempty.
 *
 * @param list a pointer to a list returned from vec_list_init()
 * @return the vector at the end of the list
 */
vector_t *vec_list_remove(vec_list_t *list) {
  assert(0 < list->length);
  list->length--;
  vector_t *removedVectored = list->arrayVector[list->length];
  return removedVectored;
}
