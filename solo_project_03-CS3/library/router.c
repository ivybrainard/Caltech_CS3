#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "router.h"
#include <assert.h>

typedef struct route {
    char *path;
    route_handler_t handler;
} route_t;

struct router {
    route_handler_t fallback;
    size_t max_routes;
    size_t num_routes;
    route_t routes[];
};



router_t *router_init(size_t max_routes, route_handler_t fallback) {
    router_t *router = malloc(sizeof(router_t) * max_routes + sizeof(router_t));
    assert(router != NULL);
    router->max_routes = max_routes;
    router->fallback = fallback;
    router->num_routes = 0;
    return router;
}


void router_register(router_t *router, const char *path, route_handler_t handler) {
    if(path == NULL) {
        return;
    }
    for(size_t i = 0; i < router->num_routes; i++) {
        if(strcmp(router->routes[i].path, path) == 0) {
            router->routes[i].handler = handler;
            return;
        }
    }
    if(router->num_routes >= router->max_routes) {
        return;
    }
    router->routes[router->num_routes].handler = handler;
    router->routes[router->num_routes].path = strdup(path);
    router->num_routes++;
}


bytes_t *router_dispatch(router_t *router, request_t *request) {
    if(router == NULL || request == NULL) {
        request_free(request);
        return NULL;
    }

    route_handler_t handler = NULL;
    for(size_t i = 0; i < router->num_routes; i++) {
        if(strcmp(router->routes[i].path, request->path)==0) {
            handler = router->routes[i].handler;
            break;
        }
    }
    bytes_t *results = NULL;
    if(handler == NULL) {
        results = router->fallback(request);
        request_free(request);
        return results;
    }
    results = handler(request);
    request_free(request);
    return results;
}

void router_free(router_t *router) {
    for(size_t i = 0; i < router->num_routes; i++) {
        free(router->routes[i].path);
    }
    free(router);
}
