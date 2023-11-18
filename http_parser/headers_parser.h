#ifndef HTTP_PARSER_HEADERS_PARSER_H
#define HTTP_PARSER_HEADERS_PARSER_H

#include "types.h"

/*
 * Init headers struct fields with their default values.
 */
void init_http_headers(http_headers_t *req_head);

/*
 * Free dynamic allocated memory for headers.
 */
void free_http_headers(http_headers_t *req_head);

/*
 * Parse http headers and fill http_headers_t.
 */
parse_log_t parse_http_req_header(char *req_txt, http_headers_t *req_head);

/*
 * hname must be the name of an http header.
 * hdata the data associated with the header name.
 * Fills req_head with those information.
 * @return 0 on success, 1 if the header is unsupported, -1 on failure.
 */
int extract_http_header(char *hname, char *hdata, http_headers_t *req_head);

#endif // HTTP_PARSER_HEADERS_PARSER_H
