#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> /* pour gethostbyname */
#include <stdlib.h>
#include <sys/select.h>

int main(int argc, char *argv[]){
    struct sockaddr_in sin, exp; /* Nom de la socket de connexion */
    int sc ; /* Socket de connexion */
    int scom; /* Socket de communication */
    struct hostent *hp;
    int fromlen = sizeof exp;
    int message;
    /* creation de la socket */
    printf("Appuyer sur une <Entree> pour tuer le serveur\n");
    /* Boucle principale */
    for (;;) {
        fd_set mselect;
        /* Construire le masque du select */
        FD_ZERO(&mselect);
        FD_SET(0,&mselect); /* stdin */
        FD_SET(sc,&mselect); /* la socket */
        struct timeval timeout;
        timeout.tv_sec = 5; /* 5 secondes */
        timeout.tv_usec = 0; /* 0 micro-seconde (10E-6 sec.) */
        if (select(sc+1, &mselect, NULL, NULL, &timeout) == -1) {
            perror("select");
            exit(3);
        }
        /*** Un evenement a eu lieu : tester le descripteur ***/
        if (FD_ISSET(0,&mselect)) {
            /* Sur stdin */
            break; /* Sortie de la boucle */
        }
        if (FD_ISSET(sc, &mselect)) {
            /* Sur la socket de connexion */
            /* Etablir la connexion */
            if ( (scom = accept(sc, (struct sockaddr *)&exp, &fromlen))== -1) {
                perror("accept"); exit(2);
            }
            /*** Lire le message ***/
            if (read(scom,message, sizeof(message)) < 0) {
                perror("read"); exit(1);
            }
            /* Fermer la connexion */
            shutdown(scom,2);
            close(scom);
        }
    } /* Fin de la boucle */
    close(sc);
    return 0;
}