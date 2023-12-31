#include "req_line_parser.h"

#include <stdlib.h>
#include <string.h>

/*
 * Init HTTP line struct fields with their default values.
 */
void init_http_req_line(http_req_t *http_req)
{
    http_req->uri.file_path = http_req->uri.query_str = NULL;
}

/*
 * Free dynamic allocated memory for request line.
 */
void free_http_req_line(http_req_t *req_struct)
{
    if (req_struct->uri.file_path != NULL)
        free(req_struct->uri.file_path);

    if (req_struct->uri.query_str != NULL)
        free(req_struct->uri.query_str);

    req_struct->uri.file_path = req_struct->uri.query_str = NULL;
}

/*
 * Parse the request line of [req_txt], and return parse logs.
 * [root_dir] is the directory from which files are served.
 * If the parse succeeds, line_end points to the character following the end of the request line.
 */
parse_log_t parse_http_req_line(char *req_txt, char *root_dir, http_req_t *req_struct)
{
    // Retrieve HTTP method
    char *token = strtok(req_txt, " ");
    if (token == NULL)
        return (PARSE_INVALID_FORMAT);

    req_struct->method = str_to_method(token);

    // Unsupported HTTP method
    if (req_struct->method == NOT_DEF)
        return (PARSE_METHOD_NOT_DEF);

    // Get HTTP URI
    token = strtok(NULL, " ");
    if (token == NULL)
        return (PARSE_INVALID_FORMAT);

    // Malformed URI
    if (extract_http_uri(token, root_dir, &req_struct->uri) == -1)
        return (PARSE_INVALID_URI_FORMAT);

    // Get HTTP version
    token = strtok(NULL, "\r");
    if (token == NULL)
    {
        free_http_req_line(req_struct);
        return (PARSE_INVALID_FORMAT);
    }

    req_struct->version = str_to_http_version(token);

    // Unsupported HTTP version
    if (req_struct->version == NOT_DEF)
    {
        free_http_req_line(req_struct);
        return (PARSE_HTTP_VERSION_NOT_DEF);
    }

    // Go to end of request line
    while (*(token++) != '\0')
        ;

    // Line must end with \r\n
    if (*token != '\n')
    {
        free_http_req_line(req_struct);
        return (PARSE_INVALID_FORMAT);
    }

    return (PARSE_OK);
}

/*
 * Return the http_method_t enum value associated with string.
 * NOT_DEF is returned if string doesn't correspond to any supported http method.
 */
http_method_t str_to_method(char *string)
{
    string = my_trim(string);

    if (strcmp(string, "GET") == 0)
        return (GET);

    else if (strcmp(string, "POST") == 0)
        return (POST);

    else
        return (NOT_DEF);
}

/*
 * [uri] must be a valid URI, and musn't be const.
 * [root_dir] is the directory from which files are served.
 * Parse the given URI and store the result in [uri_s].
 * @return 0 on success, -1 on failure.
 */
int extract_http_uri(char *uri, char *root_dir, http_uri_t *uri_s)
{
    if (uri[0] != '/')
        return (-1);

    size_t uri_len, root_dir_len;

    char *qmark = strchr(uri, '?');

    if (qmark == NULL)
        uri_s->query_str = NULL;

    else
    {
        // Parse [uri] into file_path\0query_string
        *qmark = '\0';

        // Copy query string
        uri_len = strlen(qmark + 1);
        if (uri_len == 0)
            uri_s->query_str = NULL;

        else
        {
            uri_s->query_str = malloc(uri_len + 1);
            strcpy(uri_s->query_str, qmark + 1);
        }
    }

    uri_len = strlen(uri);
    root_dir_len = strlen(root_dir);

    // No URI given
    if (uri_len == 0)
    {
        free(uri_s->query_str);
        return (-1);
    }

    // set file_path = root_dir/uri
    uri_s->file_path = malloc(root_dir_len + uri_len + 1);
    strcpy(uri_s->file_path, root_dir);
    uri_s->file_path[root_dir_len] = '/';
    strcpy(uri_s->file_path + root_dir_len + 1, uri + 1);

    return (0);
}

/*
 * Return the http_version_t enum value associated with string.
 * NOT_DEF is returned if string doesn't correspond to any supported http version.
 */
http_version_t str_to_http_version(char *string)
{
    static char http_suffix[] = "HTTP/";
    string = my_trim(string);

    /*
     * - If HTTP/ isn't a substring of [string], then strstr returns NULL, so NOT_DEF is returned.
     * - If HTTP/ is a substring of [string], but isn't an initial segment of [string], 
     *   then strstr returns a pointer not equal to [string], so NOT_DEF is returned.
     */
    if (strstr(string, http_suffix) != string)
        return (NOT_DEF);

    // Go to the http version number
    char *vers_str = string + sizeof(http_suffix) - 1;

    if (strcmp(vers_str, "1.1") == 0)
        return (HTTP_ONE_ONE);

    else if (strcmp(vers_str, "1.0") == 0)
        return (HTTP_ONE_ZERO);

    else if (strcmp(vers_str, "0.9") == 0)
        return (HTTP_ZERO_NINE);

    else
        return (NOT_DEF);
}
