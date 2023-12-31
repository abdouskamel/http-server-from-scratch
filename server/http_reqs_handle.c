#include "http_reqs_handle.h"
#include "http_response.h"

/*
 * Process the given http_req_t by sending an HTTP response to client.
 */
void process_http_req(server_info_t *serv_info, http_req_t *http_req)
{
    // Check for the existence of the requested file
    if (access(http_req->uri.file_path, F_OK) == -1)
    {
        http_response_t http_resp;
        http_resp.version = SERV_HTTP_VERSION;

        http_resp.status_code = STATUS_NOT_FOUND;
        send_http_response(serv_info->client_fd, &http_resp, NULL);

        return;
    }

    switch (http_req->method)
    {
    case GET:
        get_req(serv_info, http_req);
        break;

    case POST:
        post_req(serv_info, http_req);
        break;
    }
}

/*
 * Procedure for GET request.
 */
void get_req(server_info_t *serv_info, http_req_t *http_req)
{
    http_response_t http_resp;
    http_resp.version = SERV_HTTP_VERSION;

    http_resp.status_code = STATUS_OK;
    send_http_response(serv_info->client_fd, &http_resp, http_req->uri.file_path);
}

/*
 * Procedure for POST request.
 */
void post_req(server_info_t *serv_info, http_req_t *http_req)
{
    http_response_t http_resp;
    http_resp.version = SERV_HTTP_VERSION;

    http_resp.status_code = STATUS_OK;
    send_http_response(serv_info->client_fd, &http_resp, http_req->uri.file_path);
}
