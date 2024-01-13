#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <iostream>

void myexec (int argc, char ** args) {
    std::cerr << "args :" ;
    for (int i =0; i < argc ; i++) {
        if (args[i] != nullptr) {
            std::cerr << args[i] << " ";
        } else {
            break;
        }
    }
    std::cerr << std::endl;
    if (execv (args[0], args) == -1) {
        perror ("exec");
        exit (3);
    }
}

int main (int argc, char ** argv) {
    // On partitionne les args en deux jeux d’arguments
    char * args1[argc];
    char * args2[argc];
    memset(args1,0,argc*sizeof(char*));
    memset(args2,0,argc*sizeof(char*));
    int arg = 1;
    for ( ; arg < argc ; arg++) {
        if (! strcmp(argv[arg],"|")) {
            arg++;
            break;
        } else {
            args1[arg-1] = argv[arg];
        }
    }
    for (int i=0; arg < argc ; i++,arg++) {
        args2[i] = argv[arg];
    }
    // OK, args1 = paramètre commande 1, args2 = deuxième commande.
    int tubeDesc[2];
    pid_t pid_fils;
    if (pipe (tubeDesc) == -1) {
        perror ("pipe");
        exit (1);
    }
    if ( (pid_fils = fork ( )) == -1 ){
        perror ("fork");
        exit (2);
    }
    if (pid_fils == 0) { /* fils 1 */
        dup2(tubeDesc[1],STDOUT_FILENO);
        close (tubeDesc[1]);
        close (tubeDesc[0]);
        myexec(argc,args1);
        // on ne revient pas du exec
    }
    // pere donc ici
    if ( (pid_fils = fork ( )) == -1 ){
        perror ("fork");
        exit (2);
    }
    if (pid_fils == 0) { /* fils 2 */
        dup2(tubeDesc[0],STDIN_FILENO);
        close (tubeDesc[0]);
        close (tubeDesc[1]);
        myexec(argc,args2);
    }
    // important
    close (tubeDesc[0]);
    close (tubeDesc[1]);
    wait(0);
    wait(0);
    return 0;
}