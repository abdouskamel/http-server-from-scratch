#include "http_response.h"
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
 * Use [http_resp] to send the client an HTTP response.
 * [content_path] is the path of the file to be sent as a message body.
 * If http_resp.status_code != STATUS_OK, content_path is ignored.
 * @return 0 on success, -1 on failure.
 */
int send_http_response(int client_fd, http_response_t *http_resp, char *content_path)
{
    char buffer[RESPONSE_BUFF_LENGTH];
    int nb_char;
    char *buff_write_ptr = buffer;

    // Write response line
    nb_char = print_http_version(buff_write_ptr, http_resp->version);
    buff_write_ptr += nb_char;

    nb_char = sprintf(buff_write_ptr, "%d ", http_resp->status_code);
    buff_write_ptr += nb_char;

    nb_char = print_status_msg(buff_write_ptr, http_resp->status_code, &content_path);
    buff_write_ptr += nb_char;

    // Write response headers
    int content_fd = open(content_path, O_RDONLY);
    if (content_fd == -1)
        return (-1);

    // Get info about the content file
    struct stat content_info;
    if (fstat(content_fd, &content_info) == -1)
    {
        close(content_fd);
        return (-1);
    }

    nb_char = print_content_type(buff_write_ptr, content_path);
    buff_write_ptr += nb_char;

    nb_char = sprintf(buff_write_ptr, "Content-Length : %ld\r\n", content_info.st_size);
    buff_write_ptr += nb_char;

    // Endof headers, send response line and response headers
    nb_char = sprintf(buff_write_ptr, "\r\n");
    buff_write_ptr += nb_char;

    if (write(client_fd, buffer, strlen(buffer)) <= 0)
    {
        close(content_fd);
        return (-1);
    }

    // Write message body
    ssize_t size;
    while ((size = read(content_fd, buffer, RESPONSE_BUFF_LENGTH)) > 0)
    {
        if (write(client_fd, buffer, size) <= 0)
        {
            close(content_fd);
            return (-1);
        }
    }

    close(content_fd);
    return (0);
}

/*
 * @return Number of written characters.
 */
int print_http_version(char *buffer, http_version_t version)
{
    switch (version)
    {
    case HTTP_ZERO_NINE:
        return (sprintf(buffer, "HTTP/0.9 "));

    case HTTP_ONE_ZERO:
        return (sprintf(buffer, "HTTP/1.0 "));

    case HTTP_ONE_ONE:
        return (sprintf(buffer, "HTTP/1.1 "));

    default:
        return (-1);
    }
}

/*
 * @return Number of written characters.
 */
int print_status_msg(char *buffer, http_status_t status_code, char **content_path)
{
    switch (status_code)
    {
    case STATUS_OK:
        return (sprintf(buffer, "OK\r\n"));

    case STATUS_BAD_REQUEST:
        *content_path = "error_html_pages/bad_request.html";
        return (sprintf(buffer, "Bad Request\r\n"));

    case STATUS_NOT_FOUND:
        *content_path = "error_html_pages/not_found.html";
        return (sprintf(buffer, "Not Found\r\n"));

    case STATUS_REQ_HEADER_TOO_LARGE:
        *content_path = "error_html_pages/header_too_large.html";
        return (sprintf(buffer, "Request Header Fields Too Large\r\n"));

    case STATUS_NOT_IMPLEMENTED:
        *content_path = "error_html_pages/not_implemented.html";
        return (sprintf(buffer, "Not Implemented\r\n"));

    case STATUS_HTTP_VERSION:
        *content_path = "error_html_pages/http_version.html";
        return (sprintf(buffer, "HTTP Version Not Supported\r\n"));

    default:
        return (-1);
    }
}

/*
 * @return Number of written characters.
 */
int print_content_type(char *buffer, char *content_path)
{
    // Get file extension
    char *dot = strrchr(content_path, '.');

    // Can't find content type : no dots, dot at the beginning, dot at the end
    if (dot == NULL || dot == content_path || *(dot + 1) == '\0')
        return (sprintf(buffer, "Content-Type : application/octet-stream\r\n"));

    if (strcmp(dot, ".html") == 0 || strcmp(dot, ".htm") == 0)
        return (sprintf(buffer, "Content-Type : text/html\r\n"));

    if (strcmp(dot, ".css") == 0)
        return (sprintf(buffer, "Content-Type : text/css\r\n"));

    if (strcmp(dot, ".gif") == 0)
        return (sprintf(buffer, "Content-Type : image/gif\r\n"));

    if (strcmp(dot, ".ico") == 0)
        return (sprintf(buffer, "Content-Type : image/x-icon\r\n"));

    if (strcmp(dot, ".jpeg") == 0 || strcmp(dot, ".jpg") == 0)
        return (sprintf(buffer, "Content-Type : image/jpeg\r\n"));

    if (strcmp(dot, ".png") == 0)
        return (sprintf(buffer, "Content-Type : image/png\r\n"));

    if (strcmp(dot, ".js") == 0)
        return (sprintf(buffer, "Content-Type : application/javascript\r\n"));

    return (sprintf(buffer, "Content-Type : application/octet-stream\r\n"));
}
