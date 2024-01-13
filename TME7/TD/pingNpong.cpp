#include <fcntl.h> /* For O_* constants */
#include <sys/stat.h> /* For mode constants */
#include <semaphore.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#define N 4

int main() {
    sem_t * mutex [N];
    for (int i=0; i < N ; i++) {
        // create mutex initialisé 0 sauf le premier
        int semval = 0;
        if (i==0) {
            semval = 1;
        }
        std::string semname = "/monsem"+std::to_string(i);
        mutex[i] = sem_open(semname.c_str(), O_CREAT |O_EXCL | O_RDWR , 0666, semval);
        if (mutex[i]==SEM_FAILED) {
            perror("sem create");
            exit(1);
        }
    }
    for (int i=0; i < N ; i++) {
        pid_t f = fork();
        if (f==0) {
            // fils
            for (int round=0; round < 10; round++) {
                sem_wait(mutex[i]);
                std::cout << "Ping" << i << std::endl;
                sem_post(mutex[(i+1)%N]);
            }
            return 0;
        }
    }
    for (int i=0; i < N ; i++) {
        wait(nullptr);
    }
    // on nettoie
    for (int i=0; i < N ; i++) {
        sem_unlink( ("/monsem"+std::to_string(i)).c_str() );
    }
    return 0;
}