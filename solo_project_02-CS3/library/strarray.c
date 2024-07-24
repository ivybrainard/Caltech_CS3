#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "strarray.h"




strarray_t *strarray_init(size_t length) {
    strarray_t *b = malloc(sizeof(strarray_t));
    b->data = malloc(sizeof(char*) *length);
    b->length = length; 
    for(size_t i = 0; i < length; i++) {
        b->data[i] = NULL;
    }
    return b;

}


void strarray_free(strarray_t *arr) {

    for(size_t i = 0; i < arr->length; i++) {
        free(arr->data[i]);
    }
    
    free(arr->data);
    free(arr);
}
