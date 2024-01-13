#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <iostream>

int main () {
    pid_t P,F1,F2;
    P = getpid();
    int pdesc[2];
    if (pipe(pdesc) != 0) {
        perror("pipe error");
        return 1;
    }
    F1 = fork();
    if (F1 < 0) {
        perror("fork error");
        return 1;
    } else if (F1 == 0){
        F1 = getpid();
        // F1
        close(pdesc[1]);
        if (read(pdesc[0],&F2,sizeof(pid_t)) == sizeof(pid_t)) {
            std::cout << "Je suis :" << getpid() << " P=" << P << " F1=" << F1 <<
            " F2=" << F2 << std::endl;
        } else {
            perror("erreur read");
            return 1;
        }
        close(pdesc[0]);
        return 0;
    }

    F2 = fork();
    if (F2 < 0) {
        perror("fork error");
        return 1;
    } else if (F2 == 0){
        // F2
        F2 = getpid();
        close(pdesc[0]);
        if (write(pdesc[1],&F2,sizeof(pid_t)) == sizeof(pid_t)) {
            std::cout << "Je suis :" << getpid() << " P=" << P << " F1=" << F1 <<
            " F2=" << F2 << std::endl;
        } else {
            perror("erreur write");
            return 1;
        }
        close(pdesc[1]);
        return 0;
    }

    close(pdesc[0]);
    close(pdesc[1]);
    std::cout << "Je suis :" << getpid() << " P=" << P << " F1=" << F1 << " F2=" << F2 << std::endl;
    wait(0);
    wait(0);
    return 0;
}