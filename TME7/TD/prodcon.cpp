#include "Stack.h"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <vector>

using namespace std;
using namespace pr;

void producteur (Stack<char> * stack) {
    char c ;
    while (cin.get(c)) {
        stack->push(c);
    }
}

void consomateur (Stack<char> * stack) {
    while (true) {
        char c = stack->pop();
        cout << c << flush ;
    }
}

std::vector<pid_t> tokill;

void killall (int) {
    for (pid_t p : tokill) {
        kill(p, SIGINT);
    }
}

int main () {
    size_t shmsize = sizeof(Stack<char>);
    std::cout << " allocation segment size " << shmsize << std::endl;

    int fd;
    void * addr;

    bool useAnonyme = false;
    if(useAnonyme) {
        addr = mmap(nullptr, shmsize, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        if(addr == MAP_FAILED) {
            perror("mmap anonymous");
            exit(EXIT_FAILURE);
        }
    } else{
        fd = shm_open("/myshm", O_RDWR | O_CREAT | O_EXCL , 0666);
        if(fd < 0) {
            perror("shm_open");
            exit(EXIT_FAILURE);
        }
        if(ftruncate(fd, shmsize) == -1) {
            perror("ftruncate");
            exit(EXIT_FAILURE);
        }
        addr = mmap(nullptr, shmsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if(addr == MAP_FAILED) {
            perror("mmap");
            exit(EXIT_FAILURE);
        }
    }

    Stack<char> * s = new (addr) Stack<char>();

    pid_t pp = fork();
    if(pp == 0) {
        producteur(s);
        return 0;
    }

    pid_t pc = fork();
    if(pc == 0) {
        consomateur(s);
        return 0;
    }else{
        tokill.push_back(pc);
    }

    signal(SIGINT, killall);

    wait(0);
    wait(0);

    s->~Stack();

    if(munmap(addr, shmsize) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }
    if(!useAnonyme) {
        if(shm_unlink("/myshm") == -1) {
            perror("shm_unlink");
        }
    }

}