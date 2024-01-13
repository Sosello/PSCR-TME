#define _POSIX_SOURCE 1
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#define SIZE_TAMPON 100
char tampon [SIZE_TAMPON];

int main (int argc, char* argv []) {
    int fd1, fd2; int n,i;
    if ((fd1 = open (argv[1],O_RDWR| O_CREAT|O_SYNC,0600)) == -1) {}
    if (write (fd1,"abcdef", strlen ("abcdef"))== -1) {}
    if (fork () == 0) {
        /* fils */
        if ((fd2 = open (argv[1], O_RDWR)) == -1) {}
        if (write (fd1,"123", strlen ("123")) == -1) {}
        if ((n= read (fd2,tampon, SIZE_TAMPON)) <=0) {}
        for (i=0 ; i<n; i++)
            printf ("%c",tampon [i]);
        exit (0);
    }
    else /* père */
        wait (NULL);
    return EXIT_SUCCESS;
} 