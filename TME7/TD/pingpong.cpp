#include <fcntl.h> /* For O_* constants */
#include <sys/stat.h> /* For mode constants */
#include <semaphore.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>

int main() {
    // create mutex initialisé 0
    sem_t * smutex1 = sem_open("/monsem1", O_CREAT |O_EXCL | O_RDWR , 0666, 0);
    if (smutex1==SEM_FAILED) {
        perror("sem create");
        exit(1);
    }
    // create mutex initialisé 0
    sem_t * smutex2 = sem_open("/monsem2", O_CREAT |O_EXCL | O_RDWR , 0666, 0);
    if (smutex2==SEM_FAILED) {
        perror("sem create");
        exit(1);
    }
    pid_t f = fork();
    if (f==0) {
        // fils
        for (int i=0; i < 10; i++) {
            sem_wait(smutex1);
            std::cout << "Ping" << std::endl;
            sem_post(smutex2);
        }
        sem_close(smutex1);
        sem_close(smutex2);
    } else {
        // pere
        for (int i=0; i < 10; i++) {
            sem_post(smutex1);
            sem_wait(smutex2);
            std::cout << "Pong" << std::endl;
        }
        sem_close(smutex1);
        sem_close(smutex2);
        // on nettoie
        sem_unlink("/monsem1");
        sem_unlink("/monsem2");
        wait(nullptr);
    }
    return 0;
}