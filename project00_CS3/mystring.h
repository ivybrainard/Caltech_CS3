#ifndef MYSTRING_H
#define MYSTRING_H

typedef struct strarray {
    char **data;
    int length;
} strarray_t;

strarray_t *strsplit(const char *str);

#endif
