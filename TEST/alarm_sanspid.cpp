#include <csignal>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main () {
    int N = 4;
    std::cout << "main pid=" << getpid() << std::endl;
    int pids[N];
    for (int i =0 ; i < N ; i++ ) {
        int pid = fork();
        if (pid == 0) {
            // code fils
            std::cout << "fils " << i << " pid=" << getpid() << " ppid=" << getppid() << std::endl;
            // handler sigint
            signal(SIGINT,[](int sig) {
                std::cout << "Received SIGINT, quitting pid=" << getpid() << std::endl;
                exit(0);
            });
            // bloquer en attente de SIGINT
            sigset_t set;
            sigfillset(&set);
            sigdelset(&set,SIGINT);
            sigsuspend(&set);
        } else {
            pids[i] = pid;
        }
    }

    sigset_t set;
    sigfillset(&set);
    sigdelset(&set,SIGALRM);
    sigsuspend(&set);
    // on a ete reveille
    // se proteger du SIGINT pour ne pas s’auto kill
    signal(SIGINT, SIG_IGN);
    // ok on broadcast
    kill(0,SIGINT);
    for (int i =0 ; i < N ; i++ ) {
        wait(nullptr);
    }
    return 0;
}