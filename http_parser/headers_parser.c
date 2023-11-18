#include "headers_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Init headers struct fields to their default values.
 */
void init_http_headers(http_headers_t *req_head)
{
    req_head->host = req_head->user_agent = NULL;
    req_head->content_type = NOT_DEF;
    req_head->content_len = -1;
}

/*
 * Free dynamic allocated memory for headers.
 */
void free_http_headers(http_headers_t *req_head)
{
    if (req_head->host != NULL)
        free(req_head->host);

    if (req_head->user_agent != NULL)
        free(req_head->user_agent);

    if (req_head->content_type != NULL)
        free(req_head->content_type);

    req_head->host = req_head->user_agent = req_head->content_type = NULL;
}

/*
 * Parse http headers and fill http_headers_t.
 */
parse_log_t parse_http_req_header(char *req_txt, http_headers_t *req_head)
{
    // That's the end of the headers
    if (req_txt[0] == '\r' && req_txt[1] == '\n')
        return (PARSE_ENDOF_HEADERS);

    int i = 0;
    char *hname, *hdata;

    // We get the header name
    hname = req_txt;

    // Go to header data
    while (req_txt[i] != ':')
    {
        if (req_txt[i] == '\0' || req_txt[i] == '\r' || req_txt[i] == '\n')
            return (PARSE_INVALID_HEADER_FORMAT);

        ++i;
    }

    // Split header name and header data
    req_txt[i] = '\0';
    ++i;

    hdata = req_txt + i;

    // Go to end of header line
    while (req_txt[i] != '\r')
    {
        if (req_txt[i] == '\0' || req_txt[i] == '\n')
            return (PARSE_INVALID_HEADER_FORMAT);

        ++i;
    }

    // Header line must end with \r\n
    if (req_txt[i + 1] != '\n')
        return (PARSE_INVALID_HEADER_FORMAT);

    req_txt[i] = '\0';

    if (extract_http_header(hname, hdata, req_head) == -1)
        return (PARSE_INVALID_HEADER_FORMAT);

    return (PARSE_OK);
}

/*
 * hname must be the name of a http header.
 * hdata the data associated with the header name.
 * Fills req_head with those information.
 * @return 0 on success, 1 if the header is unsupported, -1 on failure.
 */
int extract_http_header(char *hname, char *hdata, http_headers_t *req_head)
{
    hname = my_trim(hname);
    hdata = my_trim(hdata);

    // Don't accept empty data
    size_t data_len = strlen(hdata);
    if (data_len == 0)
        return (-1);

    // The host data is stored directly as a string
    if (strcmp(hname, "Host") == 0)
    {
        if (req_head->host != NULL)
            return (-1);

        req_head->host = malloc(data_len + 1);
        strcpy(req_head->host, hdata);
    }

    // The user-agent data is stored directly as a string
    else if (strcmp(hname, "User-Agent") == 0)
    {
        if (req_head->user_agent != NULL)
            return (-1);

        req_head->user_agent = malloc(data_len + 1);
        strcpy(req_head->user_agent, hdata);
    }

    // The content-type data is stored directly as a string
    else if (strcmp(hname, "Content-Type") == 0)
    {
        if (req_head->content_type != NULL)
            return (-1);

        req_head->content_type = malloc(data_len + 1);
        strcpy(req_head->content_type, hdata);
    }

    // The content-length is stored as a positive integer
    else if (strcmp(hname, "Content-Length") == 0)
    {
        if (req_head->content_len != -1)
            return (-1);

        req_head->content_len = atoi(hdata);
        if (req_head->content_len < 0)
            return (-1);
    }

    else
        return (1);

    return (0);
}
