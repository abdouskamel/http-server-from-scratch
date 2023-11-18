#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

#include <stdlib.h>
#include "../bootstrap/types.h"

#define READ_HEADER_TO_LARGE -2
#define HEADER_LINE_MALFORMED -3

/*
 * Read a HTTP line of max length [max_len].
 * Return on success the number of read characters.
 * On failure, return READ_HEADER_TO_LARGE if the size of the header is greater
 * than HTTP_HEADER_MAX_LEN.
 * If the header line doesn't end with \r and \n, return HEADER_LINE_MALFORMED.
 * Otherwise, return (-1) for general failure.
 */
ssize_t read_http_line(int fd, char *buffer, size_t max_len);

/*
 * Clear the socket buffer for the given socket.
 */
void clear_socket_buff(int sock);

#endif // SERVER_UTILS_H
