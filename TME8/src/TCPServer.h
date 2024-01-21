#ifndef SRC_TCPSERVER_H_
#define SRC_TCPSERVER_H_

#include <thread>
#include <vector>
#include "ServerSocket.h"
#include "ConnectionHandler.h"

namespace pr {

// un serveur TCP, la gestion des connections est déléguée
class TCPServer {
	ServerSocket * ss; // la socket d'attente si elle est instanciee
	ConnectionHandler * handler; // le gestionnaire de session passe a la constru
	std::vector<std::thread> threads; // les threads de gestion de session
public :
	TCPServer(ConnectionHandler * handler): ss(nullptr),handler(handler) {}
	void handleClient(Socket scom);
	// Tente de creer une socket d'attente sur le port donné
	bool startServer0 (int port);
	bool startServer1 (int port);

	// stoppe le serveur
	void stopServer () ;
	~TCPServer();
};

} // ns pr

#endif /* SRC_TCPSERVER_H_ */
