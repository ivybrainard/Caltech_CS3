#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "strarray.h"



/**
 * Allocates a new strarray_t with the given length.
 * 
 * Both the returned struct and the data pointer are separate heap allocations,
 * so this function requires multiple calls to malloc.
 * 
 * The `data` field is a pointer to an array of strings, which are initialized 
 * to NULL.
 * 
 * The `length` field is the number of strings in the array and matches the
 * given length.
 */

strarray_t *strarray_init(size_t length) {
    strarray_t *b = malloc(sizeof(strarray_t));
    b->data = malloc(sizeof(char*) *length);
    b->length = length; 
    for(size_t i = 0; i < length; i++) {
        b->data[i] = NULL;
    }
    return b;

}

/**
 * Splits a string into an array of strings using sep as the delimiter. Leading,
 * trailing, and repeated seperators should eliminated (i.e., there should be no
 * empty strings in the output).
 * 
 * The original string is not modified. The returned array and the strings
 * inside it are heap-allocated and should be freed by the caller using
 * `strarray_free` when they are no longer needed.
 */

void strarray_free(strarray_t *arr) {

    for(size_t i = 0; i < arr->length; i++) {
        free(arr->data[i]);
    }
    
    free(arr->data);
    free(arr);
}
