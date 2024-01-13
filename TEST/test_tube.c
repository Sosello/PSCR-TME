#define _POSIX_SOURCE 1
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

#define S_BUF 100
int main (int argc, char ** argv) {
    int tubeDesc[2];
    char buffer[S_BUF];
    int n; 
    pid_t pid_fils;
    if (pipe (tubeDesc) == -1) {
        perror ("pipe"); 
        exit (1);
    }
    if ( (pid_fils = fork ( )) == -1 ) {
        perror ("fork"); 
        exit (2);
    } 
    if (pid_fils == 0) { /*fils */
        if (( n = read (tubeDesc[0],buffer, S_BUF)) == -1) {
            perror ("read"); exit (3);
        }else {
            buffer[n] = '\0'; printf ("%s\n", buffer);
        }
    exit (0);
    }
    else {/*père */
        if ( write (tubeDesc[1],"Bonjour", 7)== -1) {
            perror ("write"); exit (4);
        }
        wait (NULL);
    }
    return (EXIT_SUCCESS); 
}