#include "options.h"
#include "clientsocket.h"
#include "serversocket.h"
#include "stdmgmt.h"

int main(int argc, const char** argv) {
	printSystem("Starting ESGI http-server (version %s)...",VERSION);
	
	// Option system
	printSystem("[LOADING] Configuration...");
	initExecOptions();
	if(readExecOptions(argc, argv) != 0) {
		printError("[FATAL] Exec options reading failed.");
		return -1;
	}
	
	// if -D option is positionned, we daemonize 
	if(daemonize != 0) {
		pid_t pid = fork();
		if(pid < 0)
			printError("[FATAL] Forking fatal error !");
		else if(pid > 0)
			return 0;		
	}
	
	// Socket System
	printSystem("[LOADING] Network initialization");
	if(initServerSocket() != 0) {
		printError("[FATAL] Unable to init socket on port %d",HTTP_PORT);
		return -1;
	}
	printSystem("\033[A[LOADING] Listening");
	if(ssocketListen() != 0) {
		printError("[FATAL] Unable to listen on port %d",HTTP_PORT);
		return -1;
	}

	printSuccess("[SUCCESS] ESGI http-server is now ready !");
	
	initThreadlist();
	handleClients();
	printSystem("Shutting down ESGI http-server...");
	ssocketClose();
	printSystem("Bye.");
}
