#include "signals.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../server/utils.h"

/*
 * Handler for SIGCHLD.
 * Terminate zombies.
 */
void sigchld_handler(int sig_num)
{
    int status;

    while (waitpid(-1, &status, WNOHANG) > 0)
    {
        // Exit success
        if (WIFEXITED(status))
            close(WEXITSTATUS(status));

        else
        {
            /* TODO */
        }
    }
}
