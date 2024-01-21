#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>

int main(int argc, const char **argv)
{
    struct sockaddr_in sin;
    char host[64];
    memset((void*)&sin, 0, sizeof(sin));
    sin.sin_addr.s_addr = inet_addr(argv[1]);
    sin.sin_family = AF_INET;
    if (getnameinfo((struct sockaddr*)&sin, sizeof(sin), host, sizeof(host), NULL, NULL, 0) != 0) {
        perror("getnameinfo");
        exit(EXIT_FAILURE);
    }
    printf("Name : %s\n", host);
    return (0);
}