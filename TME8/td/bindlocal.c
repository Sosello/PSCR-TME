/* fichier "bindlocal.c" */
#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

int main(int argc, const char **argv){
    int sock;
    struct sockaddr_un addr;
    memset(&addr,'\0', sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "./MySock");
    if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1){
        perror("Erreur creation socket");
        exit(1);
    }
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1){
        perror("Erreur au nommage");
        exit(2);
    }
    return (0);
}   