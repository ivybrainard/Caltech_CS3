#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "http_response.h"

/**
 * Converts a supported status code into a human-readable brief. The brief is a
 * literal or global borrowed by the caller.
 * 
 * Supported codes are those in the `response_code_t` enum.
 * 
 * Will crash via assert, abort, or similar on unsupported status codes.
 */
static const char *status_brief(response_code_t code);
/**
 * Converts a supported type code into a HTTP mime type string.
 * 
 * Will crash via assert, abort, or similar on unsupported status codes.
 */
static const char *mime_string(mime_type_t type);
/**
 * Returns whether the specific type is a text type.
 */
static bool mime_is_text(mime_type_t type);

/**
 * Gets the length as a string of a number when serialized in base 10.
 */
static size_t base_ten_repr_len(size_t n);

static const char *status_brief(response_code_t code) {
    switch (code) {
        case HTTP_OK:
            return "OK";
        case HTTP_BAD_REQUEST:
            return "Bad Request";
        case HTTP_FORBIDDEN:
            return "Forbidden";
        case HTTP_NOT_FOUND:
            return "Not Found";
        default:
            fprintf(stderr, "status_brief: Unsupported response status code: `%d`\n", code);
            abort();
    }
}

static const char *mime_string(mime_type_t type) {
    switch (type) {
        case MIME_PLAIN:
            return "text/plain";
        case MIME_HTML: 
            return "text/html";
        case MIME_JS:
            return "text/javascript";
        case MIME_JSON:
            return "text/json";
        case MIME_PNG:
            return "image/png";
        case MIME_WASM:
            return "application/wasm";
        default:
            fprintf(stderr, "response_type_format: Unsupported mime type: `%d`\n", type);
            abort();
    }
}

static bool mime_is_text(mime_type_t type) {
    return type == MIME_PLAIN || type == MIME_HTML || type == MIME_JS;
}

static size_t base_ten_repr_len(size_t n) {
    if (n == 0) {
        return 1;
    }
    size_t len = 0;
    while (n > 0) {
        len += 1;
        n /= 10;
    }
    return len;
}

bytes_t *bytes_init(size_t len, char *data) {
    bytes_t *init = malloc(sizeof(bytes_t));
    assert(init);
    init->len = len;
    init->data = data;
    return init;
}

void bytes_free(bytes_t *bytes) {
    free(bytes->data);
    free(bytes);
}

char *response_format(response_code_t code, const char *body) {
    bytes_t *bytes = response_type_format(code, MIME_HTML, body);
    char *str = bytes->data;
    // We are not using bytes_free because do not want to free the data pointer,
    // which we just took ownership of, we just want to free the bytes_t struct
    // itself
    free(bytes);
    return str;
}

bytes_t *response_type_format(response_code_t code, mime_type_t type, const void *_body) {
    const char *body;
    size_t body_len;
    if (mime_is_text(type)) {
        if (_body == NULL) {
            body = "";
            body_len = 0;
        } else {
            body = _body;
            body_len = strlen(body);
        }
    } else {
        body = ((bytes_t *) _body)->data;
        body_len = ((bytes_t *) _body)->len;
    }
    const char *brief = status_brief(code);
    size_t brief_len = strlen(brief);
    const char *mime = mime_string(type);
    size_t mime_len = strlen(mime);
    const char FORMAT[] = 
        "HTTP/1.1 %d %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %lu\r\n"
        "\r\n";
    // length of the template in the formatted string. Subtracts off format strings.
    const size_t TEMPLATE_LEN = strlen(FORMAT) - strlen("%d") - 2 * strlen("%s") - strlen("%lu");
    const size_t STATUS_CODE_LEN = 3;
    size_t content_len_len = base_ten_repr_len(body_len);
    size_t resp_len = TEMPLATE_LEN + STATUS_CODE_LEN + brief_len + mime_len + content_len_len + body_len;
    size_t resp_size = resp_len + mime_is_text(type);
    char *resp = malloc(resp_size);
    assert(resp);
    size_t end = snprintf(resp, resp_size, FORMAT, code, brief, mime, body_len);
    memcpy(resp + end, body, body_len);
    if (mime_is_text(type)) {
        resp[resp_len] = '\0';
    }
    return bytes_init(resp_len, resp);
}