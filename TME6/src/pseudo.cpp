#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

bool timeout = false;

void handler_sigalrm(int sig) {
    std::cout << "received SIGALRM" << std::endl;
    timeout = true;
}

void handler_sigchld(int sig) {
    std::cout << "received SIGCHLD" << std::endl;
}

pid_t wait_till_pid(pid_t pid, int sec) {
    signal(SIGALRM,handler_sigalrm);
    signal(SIGCHLD,handler_sigchld);
    alarm(sec);
    sigset_t set;
    sigfillset(&set);
    sigdelset(&set,SIGALRM);
    sigdelset(&set,SIGCHLD);
    while (true) {
        std::cout << "begin" << std::endl;
        sigsuspend(&set);
        if (timeout) {
            std::cout << "Time expire for father" << std::endl;
            return -1;
        } else {
            int status;
            pid_t p = wait(&status);
            std::cout << "wait child with pid " << p << std::endl;
            if (p==pid) {
                alarm(0);
            }
            if (p==-1 || p == pid) {
                return p;
            }
        }
    }
}

int main() {
    pid_t pid =fork();
    if (pid==0) {
    
        sleep(5);
    } else {
        signal(SIGINT,[](int){});
        pid_t rval = wait_till_pid(pid,6);
        std::cout << "wait return :" << rval << std::endl;
        return 0;
    }
}