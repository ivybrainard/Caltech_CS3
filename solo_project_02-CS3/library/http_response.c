#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "http_response.h"

/**
 * Converts a supported status code into a human-readable brief.
 * 
 * Supported codes are those in the `response_code_t` enum.
 * 
 * Will crash via assert, abort, or similar on unsupported status codes.
 */
static char *status_brief(response_code_t code);

static char *status_brief(response_code_t code) {
    switch(code) {
        default:
            assert(0);
        case HTTP_OK:
            return "OK";
        case HTTP_NOT_FOUND:
            return "Not Found";
        case HTTP_BAD_REQUEST:
            return "Bad Request";
        case HTTP_FORBIDDEN:
            return "Forbidden";
    
    
    }

}

char *response_format(response_code_t code, char *body) {
    if (body == NULL) {
        body = "";
    }
    size_t body_len = strlen(body);
    char *brief = status_brief(code);
    size_t brief_len = strlen(brief);
    const char FORMAT[] = 
        "HTTP/1.1 %d %s\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "%s"
        "\r\n\r\n";
    // length of the template in the formatted string. Subtracts off format strings,
    // + 2 because sizeof accounts for null-terminators.
    const size_t TEMPLATE_LEN = sizeof(FORMAT) - sizeof("%d") - 2 * sizeof("%s") + 2;
    const size_t STATUS_CODE_LEN = 3;
    size_t resp_len = TEMPLATE_LEN + brief_len + body_len + STATUS_CODE_LEN;
    size_t resp_size = resp_len + 1;
    char *resp = malloc(resp_size);
    snprintf(resp, resp_size, FORMAT, code, brief, body);
    return resp;
}
