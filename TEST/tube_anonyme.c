#define _POSIX_SOURCE 1
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void sig_handler (int sig) {
    if (sig == SIGPIPE)
        printf ("eciture dans un tube sans lecteurs \n");
}

int main (int argc, char ** argv) {
    int tubeDesc[2]; 
    struct sigaction action;
    action.sa_handler= sig_handler;
    sigaction (SIGPIPE, &action, NULL); 
    if (pipe (tubeDesc) == - 1) {
        perror ("pipe");
        exit (1);
    }
    close (tubeDesc[0]); /* sans lecteur */
    if ( write (tubeDesc[1],"x", 1) == -1)
        perror ("write");
    return EXIT_SUCCESS;
}