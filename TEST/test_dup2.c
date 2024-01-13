#define _POSIX_SOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

int fd1;

int main (int argc, char* argv []) {
    if ((fd1 = open (argv[1], O_WRONLY| O_CREAT,0600)) == -1) {}
    printf ("avant le dup2: descripteur %d \n", fd1);
    dup2 (fd1, STDOUT_FILENO);
    printf ("après le dup2 \n");
    return EXIT_SUCCESS;
}