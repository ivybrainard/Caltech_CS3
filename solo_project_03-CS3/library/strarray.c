#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "strarray.h"
#include <assert.h>




strarray_t *strarray_init(size_t length) {
    strarray_t *array = malloc(sizeof(strarray_t));
    assert(array != NULL);
    array->data = malloc(sizeof(char*) *length);
    assert(array->data);
    array->length = length; 
    for (size_t i = 0; i < length; i++) {
        array->data[i] = NULL;
    }
    return array;
}


void strarray_free(strarray_t *arr) {
    for (size_t i = 0; i < arr->length; i++) {
        free(arr->data[i]);
    }
    free(arr->data);
    free(arr);
}