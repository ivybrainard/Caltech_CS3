#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../string_reverse.h"

const int MAX_INPUT_SIZE = 10000;

int main(int argc, char *argv[]) {
    assert(argc == 2 && "usage: test_string_reverse file");
    char *buf = malloc(MAX_INPUT_SIZE);
    FILE *in = fopen(argv[1], "r");
    assert(in && "file not found");
    buf[fread(buf, sizeof(char), MAX_INPUT_SIZE, in) - 1] = '\0'; 
    fclose(in);

    char *result = string_reverse(buf);
    printf("%s\n", result);
    free(result);
    free(buf);
}
