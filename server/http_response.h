#ifndef SERVER_HTTP_RESPONSE_H
#define SERVER_HTTP_RESPONSE_H

#include "../http_parser/types.h"

#define RESPONSE_BUFF_LENGTH 10240

typedef enum http_status_e
{
    STATUS_OK = 200,
    STATUS_BAD_REQUEST = 400,
    STATUS_NOT_FOUND = 404,
    STATUS_REQ_HEADER_TOO_LARGE = 431,
    STATUS_NOT_IMPLEMENTED = 501,
    STATUS_HTTP_VERSION = 505

} http_status_t;

typedef struct http_response_s
{
    // Response line
    http_version_t version;
    http_status_t status_code;

    // Response headers
    http_headers_t headers;

} http_response_t;

/*
 * Use [http_resp] to send the client an HTTP response.
 * [content_path] is the path of the file to be sent as a message body.
 * If http_resp.status_code != STATUS_OK, content_path is ignored.
 * @return 0 on success, -1 on failure.
 */
int send_http_response(int client_fd, http_response_t *http_resp, char *content_path);

/*
 * @return Number of written characters.
 */
int print_http_version(char *buffer, http_version_t version);

/*
 * @return Number of written characters.
 */
int print_status_msg(char *buffer, http_status_t status_code, char **content_path);

/*
 * @return Number of written characters.
 */
int print_content_type(char *buffer, char *content_path);

#endif // SERVER_HTTP_RESPONSE_H
