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

    alarm(3);
    // handler pour sigalrm
    // on pourrait aussi kill les fils dans le handler directement, mais autant eviter
    // la semantique des handlers est assez complique comme ca
    // ici un handler vide (!= SIG_IGN on prend quand même le signal !)
    signal(SIGALRM,[](int) {});
    // bloquer en attente de SIGALRM
    sigset_t set;
    sigfillset(&set);
    sigdelset(&set,SIGALRM);
    sigsuspend(&set);
    // on a été réveillé
    for (int i=0; i < N ; i++) {
        kill(pids[i],SIGINT);
    }
    for (int i =0 ; i < N ; i++ ) {
        wait(nullptr);
    }
    return 0;
}