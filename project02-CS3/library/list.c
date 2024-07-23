#include "list.h"
#include <assert.h>
#include <stdlib.h>

typedef struct list {
  size_t capacity;
  size_t size;
  void **data;
} list_t;

list_t *list_init(size_t initial_capacity) {
  list_t *array = malloc(sizeof(list_t));
  array->data = malloc(initial_capacity * sizeof(void *));
  assert(array != NULL);
  assert(array->data != NULL);
  array->capacity = initial_capacity;
  array->size = 0;
  return array;
}

void list_free(list_t *list) {
  for (size_t i = 0; i < list_size(list); i++) {
    free(list->data[i]);
  }
  free(list->data);
  free(list);
}

size_t list_size(list_t *list) { return list->size; }

void *list_get(list_t *list, size_t index) {
  assert(index < list_size(list));
  assert(index >= 0);
  void *val = list->data[index];
  return val;
}

void list_add(list_t *list, void *value) {
  assert(value != NULL);
  size_t size = list_size(list);
  size_t capacity = list->capacity;
  if (size >= capacity) {
    void **new_data = realloc(list->data, (capacity * 2) * sizeof(void *));
    assert(new_data != NULL);
    capacity *= 2;
    list->capacity = capacity;
    list->data = new_data;
  }
  list->data[size] = value;
  list->size++;
}

void *list_remove(list_t *list, size_t index) {
  size_t size = list_size(list);
  assert(index < size);
  void *temp = list->data[index];
  list->data[index] = NULL;
  for (size_t i = index; i < size - 1; i++) {
    list->data[i] = list->data[i + 1];
  }
  list->data[size - 1] = NULL;
  list->size--;
  return temp;
}
