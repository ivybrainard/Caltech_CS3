#ifndef __NETWORK_UTIL_H
#define __NETWORK_UTIL_H

#include <netdb.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <sys/select.h>


typedef struct connection connection_t;

/**
 * TODO: put docs here
 **/
typedef void nu_callback(int fd, char *str);

/**
 * Listens on localhost:port for a single client connection.
 * This function is blocking (i.e., it waits for a client to
 * connect before returning).
 **/
connection_t *nu_wait_client(int port);

/**
 * Opens a client connection to the server pointed to by ip:port.
 * This function assumes that the server has already called nu_wait_client.
 **/
connection_t *nu_connect_server(const char *ip, int port);

/** 
 * Sends a string to the open connection represented by conn.
 **/
int nu_send_str(connection_t *conn, const char *str);

/**
 * TODO: write docs here
 **/
char *nu_try_read_str(connection_t *conn);

/**
 * Reads a string sent by nu_send_str from the open 
 * connection represented by fd.
 * This function is blocking.
 **/
char *nu_read_str(connection_t *conn);

/**
 * TODO: write docs here
 * This function is blocking.
 **/
int nu_multiplex(connection_t *local, connection_t *remote, nu_callback on_local_write, nu_callback on_remote_write);

/**
 * Closes a connection opened by nu_wait_client or nu_connect_server.
 **/
void nu_close_connection(connection_t *conn);

#endif /* __NETWORK_UTIL_H */
