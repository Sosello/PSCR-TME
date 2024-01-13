#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main () {
    int N = 4;
    std::cout << "main pid=" << getpid() << std::endl;
    int pids [N];
    for (int i =0 ; i < N ; i++ ) {
        int pid = fork();
        if (pid == 0) {
            // code fils
            std::cout << "fils " << i << " pid=" << getpid() << " ppid=" <<
            getppid() << std::endl;
            return i;
        } else {
            pids[i] = pid;
        }
    }
    
    for (int i =0 ; i < N ; i++ ) {
        int status;
        int pid = waitpid(pids[i],&status,0);
        std::cout << "Fin du fils de pid=" << pid << " return " << WEXITSTATUS(
        status) << std::endl;
    }
    return 0;
}