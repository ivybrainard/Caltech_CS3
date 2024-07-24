#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

#include "network_util.h"
#include "http_request.h"
#include "http_response.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "USAGE:  %s <server port>\n", argv[0]);
        exit(1);
    }

    int port = atoi(argv[1]);

    while(1) {
        connection_t *connection = nu_wait_client(port);
        char *sever = NULL; 
        while(sever == NULL) {
            sever = nu_try_read_str(connection);
        }
        nu_send_str(connection, response_format(HTTP_OK, "Hello, world!"));
        nu_close_connection(connection);
    }
    

    return 0;
}

