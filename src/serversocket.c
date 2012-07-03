#include "prthread.h"
#include "serversocket.h"

int8_t initServerSocket() {
	
	// create server socket
	ssock_err = 0;

    ssock = socket(AF_INET, SOCK_STREAM, 0);
    if(ssock == INVALID_SOCKET)
		return SOCKET_ERROR;

	ssin.sin_addr.s_addr    = htonl(INADDR_ANY);
	ssin.sin_family         = AF_INET;
	ssin.sin_port           = htons(HTTP_PORT);

	return 0;
}

int8_t ssocketListen() {
	
	// bind & listen socket
	ssock_err = bind(ssock, (SOCKADDR*)&ssin, sizeof(ssin));
	if(ssock_err == SOCKET_ERROR)
		return SOCKET_ERROR;
		
	ssock_err = listen(ssock, 15);
	if(ssock_err == SOCKET_ERROR)
		return SOCKET_ERROR;
	
	return 0;
}

int8_t handleClients() {

	// infinite loop for listening
	while(1) {
		SOCKET csock;
		SOCKADDR_IN csin;
		socklen_t recsize = sizeof(csin);
		
		// accept clients
		csock = accept(ssock, (SOCKADDR*)&csin, &recsize);
		printDebug("Accepting Client from %s:%d (sock %d)", inet_ntoa(csin.sin_addr), htons(csin.sin_port),csock);
		
		// create client thread
		createCliThread(csock, csin, recsize);
	}
}

void ssocketClose() {
	if(ssock_err == 0)
		close(ssock);
}
