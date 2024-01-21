#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#define PORTSERV 4567 /* Port du serveur */


int main(int argc, char *argv[]){
    int reponse;
    struct sockaddr_in dest;
    int sock;
    int fromlen = sizeof(dest);
    char message[80];
    struct addrinfo *result;
    /* Le nom de la machine du serveur est passé en argument */
    if (argc != 2) {
        fprintf(stderr, "Usage : %s machine \n", argv[0]);
        exit(1);
    }
    if ((sock = socket(AF_INET,SOCK_DGRAM,0)) == -1) {
        perror("socket"); exit(1);
    }
    if (getaddrinfo(argv[1], NULL, NULL, &result) != 0) {
        perror("getaddrinfo"); 
        exit(EXIT_FAILURE);
    }
    dest.sin_addr = ((struct sockaddr_in*)result->ai_addr)->sin_addr;
    dest.sin_family = AF_INET;
    dest.sin_port = htons(PORTSERV);
    /* Contruire le message ...*/
    strcpy(message, "MESSAGE DU CLIENT");
    /* Envoyer le message */
    if (sendto(sock,message,strlen(message)+1,0,(struct sockaddr *)&dest, sizeof(dest)) == -1) {
        perror("sendto"); 
        exit(1);
    }
    /* Recevoir la reponse */
    if (recvfrom(sock,&reponse,sizeof(reponse),0,0,&fromlen) == -1) {
        perror("recvfrom"); 
        exit(1);
    }
    printf("Reponse : %d\n", reponse);
    close(sock);
    return(0);
}