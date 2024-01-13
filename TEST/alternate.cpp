#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <csignal>

int main () {
    const int NBROUND= 5;
    std::cout << "main pid=" << getpid() << std::endl;
    // pid pere
    pid_t mainpid = getpid();
    // un masque pour passer a sisgsuspend dans les deux processus
    sigset_t setneg; // neg car c’est tout sauf SIGINT
    sigfillset(&setneg);
    sigdelset(&setneg,SIGINT);
    // un handler commun aux deux processus
    signal(SIGINT,[](int sig){ 
        std::cout << "processus " << getpid() << " recu signal "
        << sig << std::endl; 
    });
    ///// BLOC SUIVANT = REPONSE QUESTION 2 ***************
    // proteger le fils du premier signal avant sigsuspend
    sigset_t setpos; // pos car c’est seulement SIGINT
    
    sigemptyset(&setpos);
    sigaddset(&setpos,SIGINT);
    // SIG_BLOCK = ajouter au masque actif
    // l’ensemble de signaux a ajouter = setpos
    // l’ancien masque si ca nous interesse, ou nullptr
    sigprocmask(SIG_BLOCK,&setpos,nullptr);
    
    //////////////////////////////////********************
    // pid fils ou 0
    pid_t pid =fork();
    if (pid == 0) {
    // code fils
        std::cout << "Naissance fils "<< getpid() << std::endl;
        for (int i = 0 ; i < NBROUND ; i++) {
            sigsuspend(& setneg);
            std::cout << "fils " << i << " pid=" << getpid() << " ppid=" <<
            getppid() << " Tour : " << i << std::endl;
            kill(mainpid,SIGINT);
        }
        std::cout << "Mort du fils" << std::endl;
        return 0;
    } else {
    // donner du temps au fils pour naitre et atteindre sigsuspend
    // En question 1 : on fait un sleep ici. Supprime en question 2
    //sleep(1);
    // code pere
        for (int i = 0 ; i < NBROUND ; i++) {
            std::cout << "Pere " << i << " pid=" << getpid() << " Tour : " << i << std::endl;
            // si le fils n’existe pas encore ici, on perd le premier signal
            kill(pid,SIGINT);
            sigsuspend(& setneg);
        }
            wait(nullptr);
        std::cout << "Mort du pere" << std::endl;
    }
    return 0;
}