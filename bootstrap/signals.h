#ifndef BOOTSTRAP_SIGNALS_H
#define BOOTSTRAP_SIGNALS_H

/*
 * Handler for SIGCHLD.
 * Terminate zombies.
 */
void sigchld_handler(int sig_num);

#endif // BOOTSTRAP_SIGNALS_H
