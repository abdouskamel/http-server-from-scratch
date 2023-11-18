#ifndef BOOTSTRAP_INIT_H
#define BOOTSTRAP_INIT_H

#include "types.h"

#define LISTEN_BACKLOG 1024

/*
 * Install process signals.
 * @return 0 on success. On failure, returns the number of the signal that caused the error.
 */
int init_signals();

/*
 * Open socket serv_ip:port as the HTTP listening socket.
 * root_dir is the directory from which files are served.
 * [serv_ip] must be an ipv4 address, in application form.
 * @return 0 on success, -1 on failure.
 */
int init_server(char *serv_ip, in_port_t port, char *root_dir, server_info_t *serv_info);

/*
 * Free server resources.
 */
void free_server(server_info_t *serv_info);

#endif // BOOTSTRAP_INIT_H
