#define _POSIX_SOURCE 1
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#define S_BUF 100
int n ; 
char buffer[S_BUF];

int main (int argc, char ** argv) {
    int fd_read;
    if (( fd_read = open (argv[1],O_RDONLY)) == -1) {
        perror ("open"); 
        exit (1);
    }
    if (( n= read (fd_read, buffer, S_BUF)) == -1){
        perror ("read");
        exit (2);
    }else {
        buffer[n] = '\0';
        printf ("%s\n",buffer);
    }
    close (fd_read);
    return EXIT_SUCCESS;
}