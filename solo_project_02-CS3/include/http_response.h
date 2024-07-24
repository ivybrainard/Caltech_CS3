#ifndef __HTTP_RESPONSE_H
#define __HTTP_RESPONSE_H

/**
 * An enumeration of supported HTTP response codes.
 * 
 * See https://developer.mozilla.org/en-US/docs/Web/HTTP/Status for more details.
 */
typedef enum response_code {
    HTTP_OK = 200,          // brief: OK
    HTTP_BAD_REQUEST = 400, // brief: Bad Request
    HTTP_FORBIDDEN = 403,   // brief: Forbidden
    HTTP_NOT_FOUND = 404,   // brief: Not Found
} response_code_t;

/**
 * Returns an owned response string for a given code and, optionally, body. If
 * `body` is `NULL`, it is omitted.
 * 
 * `body` is borrowed and the caller is responsible for freeing if necessary.
 * 
 * The response is formatted as 
 * [HTTP Version] [Response code] [Response brief]\r\n
 * [Header key 1]: [Value 1]\r\n
 * ...
 * [Header key n]: [Value n]\r\n
 * \r\n
 * [body]
 * 
 * This function only supports HTTP/1.1 for version and outputs only the header
 * Content-Type: text/html
 * 
 * The human readable [Response briefs] are those documented in `response_code_t`.
 */
char *response_format(response_code_t code, char *body);

#endif // __HTTP_RESPONSE_H