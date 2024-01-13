#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <csignal>
#include <ctime>
#include <cstdlib>
#include <time.h>
#include <cstdio>
#include "rsleep.h"

sig_atomic_t vie = 10;

void handler(int sig){
    if(sig == SIGINT){
        vie--;
        std::cout << "Vie de " << getpid() << " : " << vie << std::endl;
        if(vie == 0){
            std::cout << "Mort de " << getpid() << std::endl;
            exit(1);
        }
    }
}

void attaque (pid_t adversaire){
    signal(SIGINT, handler);
    std::cout << "Attaque de " << getpid() << " sur " << adversaire << std::endl;
    if(kill(adversaire, SIGINT)<0){
        //std::cout << "Mort de " << adversaire << std::endl;
        exit(1);
    }
    randsleep();
}

void defense(){
    signal(SIGINT, SIG_IGN);
    randsleep();
}

void combat(pid_t adversaire){
    std::cout << "Debut du combat entre " << getpid() << " et " << adversaire << std::endl;
    while(true){
        defense();
        attaque(adversaire);
    }
}

int main(int argc, char** argv){
    pid_t Vador = getpid();
    pid_t Luke = fork();
    std::cout << "Vador : " << Vador << std::endl;
    srand(Luke);
    if(!Luke){
        combat(Vador);
    }
    else{
        combat(Luke);
    }
    return 0;
}