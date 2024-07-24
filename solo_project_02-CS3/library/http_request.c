#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "http_request.h"
#include "ll.h"
#include "mystr.h"

request_t *request_init(const char *method, const char *path , const char *http_version) {
    request_t *request = malloc(sizeof(request_t));
    request->method = strdup(method);
    request->path = strdup(path);
    request->http_version = strdup(http_version);
    request->headers = ll_init();
    return request;
}



request_t *request_parse(const char *contents) {
    strarray_t *array = mystr_split(contents, '\n');
    strarray_t *header = mystr_split(array->data[0], ' ');
    strarray_t *rest = mystr_split(header->data[2], '\r');
    request_t *parse = request_init(header->data[0], header->data[1], rest->data[0]);

    for(size_t i = 1; i < array->length - 1; i++){
        char *new_line = array->data[i];
        size_t idx_key = mystr_indexof(new_line, ':', 0);
        char *key = strndup(new_line, idx_key);
        char *value = strndup(new_line + idx_key + 2, strlen(new_line) - idx_key -3);
        ll_put(parse->headers, key, value);
    }

    strarray_free(array);
    strarray_free(header);
    strarray_free(rest);
    
    return parse;
}

void request_free(request_t *req) {
    free(req->method);
    free(req->path);
    free(req->http_version);
    ll_free(req->headers);
    free(req);
}