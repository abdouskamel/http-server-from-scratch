#include "types.h"
#include "req_line_parser.h"
#include "headers_parser.h"

#include <ctype.h>

/*
 * Removes leading and tailing whitespaces of [str].
 * Returns a pointer to the first character of new string.
 */
char *my_trim(char *str)
{
    while (*str == ' ' && *str != '\0')
        ++str;

    char *tmp = str;
    while (*tmp != ' ' && *tmp != '\0')
        ++tmp;

    *tmp = '\0';

    return str;
}

/*
 * Free dynamically allocated memory for http_req_t.
 */
void free_http_req(http_req_t *http_req)
{
    free_http_req_line(http_req);
    free_http_headers(&http_req->headers);
}
