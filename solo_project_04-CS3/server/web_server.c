#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

#include "network_util.h"
#include "http_request.h"
#include "http_response.h"
#include "router.h"
#include "web_util.h"
#include "assert.h"



bytes_t *hello_handler() {
    return response_type_format(HTTP_OK, MIME_HTML, bytes_init(13, "Hello, world!"));
}

bytes_t *roll_handler() {
    size_t random_number = rand() % 6 + 1;
    char number_store[32];
    sprintf(number_store, "%zu", random_number);
    return response_type_format(HTTP_OK, MIME_HTML, bytes_init(strlen(number_store), number_store));
}

bytes_t *default_handler(request_t *request) {
    char *path = wutil_get_resolved_path(request);
    if (path == NULL) {
        return response_type_format(HTTP_FORBIDDEN, MIME_PLAIN, bytes_init(6, "Error!"));
    }
    response_code_t response =  wutil_check_resolved_path(path);
    if (response != HTTP_OK) {
        return response_type_format(response, MIME_PLAIN, bytes_init(6, "Error!"));
    }
    FILE *file = fopen(path, "r");
    size_t file_size = wutil_get_file_size(file);
    char *content = (char *)malloc(file_size);
    assert(content != NULL);
    fread(content, 1, file_size, file);
    char *exit = wutil_get_filename_ext(path);
    mime_type_t mime_type = wutil_get_mime_from_extension(exit); 
    return response_type_format(HTTP_OK, mime_type, bytes_init(file_size, content));

}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "USAGE:  %s <server port>\n", argv[0]);
        exit(1);
    }

    int port = atoi(argv[1]);
    router_t *router = router_init(2, default_handler);
    router_register(router, "/hello", hello_handler);
    router_register(router, "/roll", roll_handler);

    while (1) {
        connection_t *connection = nu_wait_client(port);
        char *sever = NULL; 
        while (sever == NULL) {
            sever = nu_try_read_header(connection);
        }
        request_t *request = request_parse(sever);
        bytes_t *response = router_dispatch(router, request);
        nu_send_bytes(connection, response->data, response->len);
        nu_close_connection(connection);
    }
    return 0;
}
