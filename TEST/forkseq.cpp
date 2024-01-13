#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main () {
    int N = 4;
    std::cout << "main pid=" << getpid() << std::endl;
    int i = 0;
    for ( ; i < N ; i++ ) {
        int pid = fork();
        if (pid == 0) {
            // code fils
            std::cout << "fils " << i << " pid=" << getpid() << " ppid=" <<
            getppid() << std::endl;
        } else {
            break;
        }
    }

    if (i != N) {
        int status;
        int pid = wait(&status);
        std::cout << "Detection par " << getpid() << " Fin du fils de pid=" << pid
        << " return " << WEXITSTATUS(status) << std::endl;
    }
    return 0;
}