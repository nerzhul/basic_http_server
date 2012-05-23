#include "clientsocket.h"
#include "serversocket.h"
#include "stdmgmt.h"
#include <dirent.h>

int main(int argc, const char** argv) {
	printSystem("Starting ESGI http-server (version %s)...",VERSION);
	if(initServerSocket() != 0) {
		printError("Unable to init socket on port %d",HTTP_PORT);
		return -1;
	}
	if(ssocketListen() != 0) {
		printError("Unable to listen on port %d",HTTP_PORT);
		return -1;
	}
	
	initThreadlist();
	handleClients();
	printSystem("Shutting down ESGI http-server...");
	ssocketClose();
	printSystem("Bye.");
}
