#include "TCPServer.h"
#include "ConnectionHandler.h"
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <sys/select.h>

namespace pr {
void TCPServer::handleClient(Socket scom) {
    int lu;
    read(scom.getFD(),&lu,sizeof(lu));
    ++lu;
    write(scom.getFD(),&lu,sizeof(lu));
    scom.close();
}

bool TCPServer::startServer0(int port) {
    ss = new ServerSocket(port);
    if (ss->isOpen()) {
        while(1){
            std::cout << "Attente d'un client" << std::endl;
            Socket scom = ss->accept();
            if(scom.isOpen()){
                std::cout << "Client connecté" << std::endl;
                handleClient(scom);
            }
            return true;
        }
    }
    return false;
}

bool TCPServer::startServer1(int port) {
    ss = new ServerSocket(port);
    if (ss->isOpen()) {
        while(1){
            std::cout << "Attente d'un client" << std::endl;
            Socket scom = ss->accept();
            if(scom.isOpen()){
                std::cout << "Client connecté" << std::endl;
                threads.emplace_back(&TCPServer::handleClient,this,scom);
            }
            return true;
        }
    }
    return false;
}

void TCPServer::stopServer() {
    ss->close();
}

TCPServer::~TCPServer() {
    for(auto & t : threads){
        t.join();
    }
}

}