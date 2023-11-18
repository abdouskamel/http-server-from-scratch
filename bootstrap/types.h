#ifndef BOOTSTRAP_TYPES_H
#define BOOTSTRAP_TYPES_H

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#include "../http_parser/types.h"

#define SERV_HTTP_VERSION HTTP_ONE_ONE

typedef struct server_info_s
{
    struct sockaddr_in sock_info;
    int sock_fd;

    struct sockaddr_in client_info;
    int client_fd;

    // Used for printing server logs
    FILE *log_file;

} server_info_t;

#endif // BOOTSTRAP_TYPES_H
