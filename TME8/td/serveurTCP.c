#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> /* pour gethostbyname */
#include <stdlib.h>

#define PORTSERV 7100
int main(int argc, char *argv[]){
    struct sockaddr_in sin, exp; /* Nom de la socket de connexion */
    int sc ; /* Socket de connexion */
    int scom; /* Socket de communication */
    struct hostent *hp;
    int fromlen = sizeof exp;
    int cpt;
    /* creation de la socket */
    if ((sc = socket(AF_INET,SOCK_STREAM,0)) < 0) {
        perror("socket"); 
        exit(1);
    }
    memset((char *)&sin,0, sizeof(sin));
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(PORTSERV);
    sin.sin_family = AF_INET;
    /* nommage */
    if (bind(sc,(struct sockaddr *)&sin,sizeof(sin)) < 0) {
        perror("bind");
        exit(1);
    }
    listen(sc, 5);
    for (;;) {
        if ((scom = accept(sc, (struct sockaddr *)&exp, &fromlen))== -1) {
            perror("accept"); 
            exit(3);
        }
        /* Création d'un processus fils qui traite la requete */
        if (fork() == 0) {
            /* Processus fils */
            if (read(scom,&cpt, sizeof(cpt)) < 0) {
                perror("read"); 
                exit(4);
            }
            /*** Traitement du message ***/
            cpt++;
            if (write(scom, &cpt, sizeof(cpt)) == -1) {
                perror("write"); 
                exit(2);
            }
            /* Fermer la communication */
            shutdown(scom,2);
            close(scom);
            exit(0);
        }
    } /* Fin de la boucle for */
    close(sc);
    return 0;
} 