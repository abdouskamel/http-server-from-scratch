#ifndef SERVER_HTTP_REQS_H
#define SERVER_HTTP_REQS_H

#include "../http_parser/types.h"
#include "../bootstrap/types.h"

/*
 * Process given http_req_t by sending HTTP response to client.
 */
void process_http_req(server_info_t *serv_info, http_req_t *http_req);

/*
 * Procedure for GET request.
 */
void get_req(server_info_t *serv_info, http_req_t *http_req);

/*
 * Procedure for POST request.
 */
void post_req(server_info_t *serv_info, http_req_t *http_req);

#endif // SERVER_HTTP_REQS_H
