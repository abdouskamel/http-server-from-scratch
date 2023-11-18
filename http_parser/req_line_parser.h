#ifndef HTTP_PARSER_REQ_LINE_PARSER_H
#define HTTP_PARSER_REQ_LINE_PARSER_H

#include "types.h"

/*
 * Init HTTP line struct fields to their default values.
 */
void init_http_req_line(http_req_t *http_req);

/*
 * Free dynamic allocated memory for request line.
 */
void free_http_req_line(http_req_t *req_struct);

/*
 * Parse the request line of [req_txt], by returning parse logs.
 * If the parse succeeds, line_end points to the character following the end of
 * the request line.
 */
parse_log_t parse_http_req_line(char *req_txt, http_req_t *req_struct);

/*
 * Return the http_method_t enum value associated with string.
 * NOT_DEF is  returned if string doesn't correspond to any supported http method.
 */
http_method_t str_to_method(char *string);

/*
 * [string] must be a valid URI, and musn't be const.
 * Parse the given URI and store the result in [uri_s].
 * @return 0 on success, -1 on failure.
 */
int extract_http_uri(char *string, http_uri_t *uri_s);

/*
 * Return the http_version_t enum value associated with string.
 * NOT_DEF is  returned if string doesn't correspond to any supported http version.
 */
http_version_t str_to_http_version(char *string);

#endif // HTTP_PARSER_REQ_LINE_PARSER_H
