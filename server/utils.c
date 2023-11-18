#include "utils.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define EMPTY_SOCK_BUFF_LEN 1024

/*
 * Read a HTTP line of max length [max_len].
 * Return on success the number of read characters.
 * On failure, return READ_HEADER_TO_LARGE if the size of the header is greater
 * than max_len.
 * If the header line doesn't end with \r and \n, return HEADER_LINE_MALFORMED.
 * Otherwise, return (-1) for general failure.
 */
ssize_t read_http_line(int fd, char *buffer, size_t max_len)
{
    int i = 0;

    while (read(fd, buffer + i, 1) > 0)
    {
        if (buffer[i] == '\r')
        {
            ++i;

            if (read(fd, buffer + i, 1) > 0)
            {
                if (buffer[i] == '\n')
                {
                    buffer[i + 1] = '\0';
                    return (i + 1);
                }

                else
                    return (HEADER_LINE_MALFORMED);
            }

            else
                return (-1);
        }

        ++i;
        if (i >= max_len - 1)
            return (READ_HEADER_TO_LARGE);
    }

    if (i == 0)
        return (0);

    else
        return (-1);
}

/*
 * Clear the socket buffer for the given socket.
 */
void clear_socket_buff(int sock)
{
    int one = 1;
    ioctl(sock, FIONBIO, &one);

    int buffer[EMPTY_SOCK_BUFF_LEN];
    while (read(sock, buffer, EMPTY_SOCK_BUFF_LEN) > 0)
        ;

    int zero = 0;
    ioctl(sock, FIONBIO, &zero);
}
