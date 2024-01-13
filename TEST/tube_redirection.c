#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main (int argc, char ** argv) {
    int tubeDesc[2]; pid_t pid_fils;
    if (pipe (tubeDesc) == -1) {
        perror ("pipe");
        exit (1);
    }
    if ( (pid_fils = fork ( )) == -1 ){
        perror ("fork");
        exit (2);
    } 
    if (pid_fils == 0) { /* fils */
        dup2(tubeDesc[1],STDOUT_FILENO);
        close (tubeDesc[1]); 
        close (tubeDesc[0]);
        if (execl ("/bin/cat", "cat", "file1.dat",NULL) == -1) {
            perror ("execl"); 
            exit (3);
        }
    }else { /* père */
        dup2(tubeDesc[0],STDIN_FILENO);
        close (tubeDesc[0]);
        close (tubeDesc[1]);
        if (execl ("/bin/wc", "wc", "-l", NULL) == -1) {
            perror ("execl"); 
            exit (3);
        }
    }
    return (EXIT_SUCCESS);
}