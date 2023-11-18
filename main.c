#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <errno.h>

#include "bootstrap/init.h"
#include "server/launch.h"  

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage : <ipv4_address> <port>\n");
        return (EXIT_FAILURE);
    }

    int16_t in_port = atoi(argv[2]);
    if (in_port <= 0)
    {
        fprintf(stderr, "<port> must be a positive integer.\n");
        return (EXIT_FAILURE);
    }

    if (init_signals() != 0)
    {
        fprintf(stderr, "Can't init signals : %s.\n", strerror(errno));
        return (EXIT_FAILURE);
    }

    server_info_t serv_info;
    if (init_server(argv[1], in_port, &serv_info) == -1)
    {
        fprintf(stderr, "Can't init HTTP server : %s.\n", strerror(errno));
        return (EXIT_FAILURE);
    }

    serve_forever(&serv_info);
    free_server(&serv_info);

    return (EXIT_SUCCESS);
}
