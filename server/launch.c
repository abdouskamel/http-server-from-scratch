#include "launch.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "http_reqs_handle.h"
#include "http_response.h"

/*
 * Launch the configured HTTP server [serv_info].
 * The program will remain as a server until it is interrupted by a signal.
 */
void serve_forever(server_info_t *serv_info)
{
    socklen_t client_addrlen;

    while (1)
    {
        // Wait for a client connection
        client_addrlen = sizeof(serv_info->client_info);
        serv_info->client_fd = accept(serv_info->sock_fd, (struct sockaddr *)&serv_info->client_info, &client_addrlen);

        if (serv_info->client_fd == -1)
        {
            fprintf(serv_info->log_file, "Error on accepting a request : %s.\n", strerror(errno));
            continue;
        }

        // Fork a server process for this client
        if (!fork())
        {
            serve_client(serv_info);

            // We clear the receiving buffer for avoiding TCP RST
            clear_socket_buff(serv_info->client_fd);
            close(serv_info->client_fd);

            // Return the file descriptor to the parent so it can close the socket
            exit(serv_info->client_fd);
        }
    }
}

/*
 * Serve a client, with information stored in [serv_info].
 */
void serve_client(server_info_t *serv_info)
{
    // First thing to do, read client HTTP request
    char buffer[HTTP_HEADER_MAX_LEN + 1];
    ssize_t size;
    parse_log_t plog;
    http_req_t http_req;

    // Read the request line
    init_http_req_line(&http_req);
    size = http_socket_read(serv_info->client_fd, buffer);
    if (size == -1)
        return;

    plog = parse_http_req_line(buffer, &http_req);
    if (manage_parse_logs(plog, serv_info->client_fd) == -1)
    {
        close(serv_info->client_fd);
        return;
    }

    // Read headers
    init_http_headers(&http_req.headers);

    int i = 0;
    while (i <= HTTP_MAX_HEADERS_NB)
    {
        size = http_socket_read(serv_info->client_fd, buffer);
        if (size == 0)
            return;

        plog = parse_http_req_header(buffer, &http_req.headers);
        if (plog == PARSE_ENDOF_HEADERS)
            break;

        if (manage_parse_logs(plog, serv_info->client_fd) == -1)
        {
            close(serv_info->client_fd);
            return;
        }

        ++i;
    }

    // Request exceed max headers number
    if (i > HTTP_MAX_HEADERS_NB)
    {
        http_response_t http_resp;
        http_resp.version = SERV_HTTP_VERSION;

        http_resp.status_code = STATUS_REQ_HEADER_TOO_LARGE;
        send_http_response(serv_info->client_fd, &http_resp, NULL);

        free_http_req(&http_req);
        close(serv_info->client_fd);
        return;
    }

    // We can now process client request
    process_http_req(serv_info, &http_req);

    free_http_req(&http_req);
}

/*
 * Read a HTTP line of max length HTTP_HEADER_MAX_LEN and store it in buffer.
 * On success return the size of the readed line.
 * On failure, send a response to the client, close the socket and return -1.
 */
ssize_t http_socket_read(int client_fd, char *buffer)
{
    ssize_t size = read_http_line(client_fd, buffer, HTTP_HEADER_MAX_LEN);

    http_response_t http_resp;
    http_resp.version = SERV_HTTP_VERSION;

    switch (size)
    {
    case 0:
        close(client_fd);
        return (-1);

    case READ_HEADER_TO_LARGE:
        http_resp.status_code = STATUS_REQ_HEADER_TOO_LARGE;
        send_http_response(client_fd, &http_resp, NULL);
        close(client_fd);
        return (-1);

    case HEADER_LINE_MALFORMED:
        http_resp.status_code = STATUS_BAD_REQUEST;
        send_http_response(client_fd, &http_resp, NULL);
        close(client_fd);
        return (-1);

    default:
        return (size);
    }
}

/*
 * If log_num == PARSE_OK, return 0.
 * Otherwise, send to the client a HTTP response indicating error, and return -1.
 */
int manage_parse_logs(parse_log_t log_num, int client_fd)
{
    http_response_t http_resp;
    http_resp.version = SERV_HTTP_VERSION;

    switch (log_num)
    {
    case PARSE_OK:
        return (0);

    case PARSE_INVALID_FORMAT:
    case PARSE_INVALID_URI_FORMAT:
    case PARSE_INVALID_HEADER_FORMAT:
        http_resp.status_code = STATUS_BAD_REQUEST;
        send_http_response(client_fd, &http_resp, NULL);
        return (-1);

    case PARSE_METHOD_NOT_DEF:
        http_resp.status_code = STATUS_NOT_IMPLEMENTED;
        send_http_response(client_fd, &http_resp, NULL);
        return (-1);

    case PARSE_HTTP_VERSION_NOT_DEF:
        http_resp.status_code = STATUS_HTTP_VERSION;
        send_http_response(client_fd, &http_resp, NULL);
        return (-1);

    default:
        return (0);
    }
}
