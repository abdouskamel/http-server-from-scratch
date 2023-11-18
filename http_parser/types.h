#ifndef HTTP_PARSER_TYPES_H
#define HTTP_PARSER_TYPES_H

#include <stdint.h>

#define HTTP_HEADER_MAX_LEN 8192
#define HTTP_MAX_HEADERS_NB 100

#define NOT_DEF 0

/*
 * Supported HTTP methods.
 */
typedef enum http_method_e
{
    GET = 1,
    POST

} http_method_t;

/*
 * Supported HTTP versions.
 */
typedef enum http_version_e
{
    HTTP_ZERO_NINE = 1, /* HTTP/0.9 */
    HTTP_ONE_ZERO,      /* HTTP/1.0 */
    HTTP_ONE_ONE        /* HTTP/1.1 */

} http_version_t;

/*
 * An URI consists of two parts :
 * 1. The requested file path.
 * 2. Arguments for dynamic content.
 */
typedef struct http_uri_s
{
    char *file_path,
        *query_str;

} http_uri_t;

/*
 * Headers of http requests and http responses.
 */
typedef struct http_headers_s
{
    char *host,
        *user_agent,
        *content_type;

    int32_t content_len;

} http_headers_t;

/*
 * Used to handle parsed http requests.
 */
typedef struct http_req_s
{
    // Request line
    http_method_t method;
    http_uri_t uri;
    http_version_t version;

    // Request headers
    http_headers_t headers;

} http_req_t;

/*
 * Define values that parse_http_req can return.
 */
typedef enum parse_log_e
{
    PARSE_OK,
    PARSE_INVALID_FORMAT,
    PARSE_INVALID_URI_FORMAT,
    PARSE_INVALID_HEADER_FORMAT,
    PARSE_METHOD_NOT_DEF,
    PARSE_HTTP_VERSION_NOT_DEF,
    PARSE_ENDOF_HEADERS

} parse_log_t;

/*
 * Removes leading and tailing whitespaces of [str].
 * Returns a pointer to the first character of new string.
 */
char *my_trim(char *str);

/*
 * Free dynamically allocated memory for http_req_t.
 */
void free_http_req(http_req_t *http_req);

#endif // HTTP_PARSER_TYPES_H
