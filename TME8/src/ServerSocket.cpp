#include "ServerSocket.h"
#include <cstring>  
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>

namespace pr{

    ServerSocket::ServerSocket(int port):socketfd(-1){
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        if(fd == -1){
            throw std::runtime_error("socket failed");
            return;
        }

        struct sockaddr_in sin;
        memset(&sin, 0, sizeof sin);
        sin.sin_family = AF_INET;
        sin.sin_port = htons(port);
        sin.sin_addr.s_addr = htonl(INADDR_ANY);

        if(bind(fd, (struct sockaddr *)&sin, sizeof sin) < 0){
            throw std::runtime_error("bind failed");
            ::close(fd);
            return;
        }

        if(listen(fd, 50) < 0){
            throw std::runtime_error("listen failed");
            ::close(fd);
            return;
        }

        socketfd = fd;

    }

    Socket ServerSocket::accept(){
        struct sockaddr_in exp;
        socklen_t len = sizeof exp;
        int fd = ::accept(socketfd, (struct sockaddr *)&exp, &len);
        if(fd < 0){
            throw std::runtime_error("accept failed");
            return Socket();
        }else{
            char ipstr[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(exp.sin_addr), ipstr, sizeof(ipstr));
            std::cout << "Connection from " << ipstr << std::endl;
        }

        return Socket(fd);
    }

    void ServerSocket::close(){
        if(socketfd != -1){
            ::close(socketfd);
            socketfd = -1;
        }
    }


}