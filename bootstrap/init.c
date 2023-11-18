#include "init.h"
#include "signals.h"

/*
 * Install process signals.
 * @return 0 on success. On failure, returns the number of the signal that caused the error.
 */
int init_signals()
{
    struct sigaction install_s;

    sigemptyset(&install_s.sa_mask);
    install_s.sa_flags = SA_RESTART;

    install_s.sa_handler = sigchld_handler;
    if (sigaction(SIGCHLD, &install_s, NULL) == -1)
        return (SIGCHLD);

    return (0);
}

/*
 * Open socket serv_ip:port as the HTTP listening socket.
 * [root_dir] is the directory from which files are served.
 * [serv_ip] must be an ipv4 address, in application form.
 * @return 0 on success, -1 on failure.
 */
int init_server(char *serv_ip, in_port_t port, char *root_dir, server_info_t *serv_info)
{
    serv_info->sock_info.sin_family = AF_INET;
    serv_info->sock_info.sin_port = htons(port);
    serv_info->root_dir = root_dir;

    if (inet_aton(serv_ip, &serv_info->sock_info.sin_addr) == 0)
        return (-1);

    serv_info->sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_info->sock_fd == -1)
        return (-1);

    if (bind(serv_info->sock_fd, (struct sockaddr *)&serv_info->sock_info, sizeof(serv_info->sock_info)) == -1)
    {
        close(serv_info->sock_fd);
        return (-1);
    }

    if (listen(serv_info->sock_fd, LISTEN_BACKLOG) == -1)
    {
        close(serv_info->sock_fd);
        return (-1);
    }

    serv_info->log_file = stderr;
    return (0);
}

/*
 * Free server resources.
 */
void free_server(server_info_t *serv_info)
{
    close(serv_info->sock_fd);
}
