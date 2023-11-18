#ifndef SERVER_LAUNCH_H
#define SERVER_LAUNCH_H

#include "../bootstrap/types.h"
#include "../http_parser/req_line_parser.h"
#include "../http_parser/headers_parser.h"
#include "utils.h"

/*
 * Launch the configured HTTP server [serv_info].
 * The program will remain as a server until it is interrupted by a signal.
 */
void serve_forever(server_info_t *serv_info);

/*
 * Serve a client, with information stored in [serv_info].
 */
void serve_client(server_info_t *serv_info);

/*
 * Read an HTTP line with a max length of HTTP_HEADER_MAX_LEN and store it in buffer.
 * On success return the size of the line.
 * On failure, send a response to the client, close the socket and return -1.
 */
ssize_t http_socket_read(int client_fd, char *buffer);

/*
 * If log_num == PARSE_OK, return 0.
 * Otherwise, send to the client an HTTP response with the error code, and return -1.
 */
int manage_parse_logs(parse_log_t log_num, int client_fd);

#endif // SERVER_LAUNCH_H
