#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "strarray.h"
#include "mystr.h"
#include <assert.h>



ssize_t mystr_indexof(const char *str, const char sep, size_t start) {
    for (size_t i = 0; i < strlen(str); i++) {
        if (str[i] == sep) {
          if (start <= i) { 
            return i;
           }
        }
    }
    return -1;
}


strarray_t *mystr_split(const char *str, const char sep) {
  int word_count = 0;
  int idx = 0;
  while (str[idx] != '\0') {
    while (str[idx] == sep) {
      idx++;
    }
    if (str[idx] == '\0') {
      break;
    }
    word_count++;
    while (str[idx] != sep && str[idx] != '\0') {
      idx++;
    }
  }
  strarray_t *array = malloc(sizeof(strarray_t));
  array->length = word_count;
  array->data = malloc(word_count * sizeof(char *));
  assert(array->data != NULL);
  int num = 0;
  int num2 = 0;
  while (str[num] != '\0') {
    if (str[num] != sep) {
      int start = num;
      while (str[num] != sep && str[num] != '\0') {
        num++;
      }
      int length = num - start;
      array->data[num2] = malloc(length + 1);
      strncpy(array->data[num2], str + start, length);
      array->data[num2][length] = '\0';
      num2++;
    } else {
      num++;
    }
  }
  return array;
}
