#include "clientsocket.h"
#include "prthread.h"

void* clientHandler(void* data) {
	prThread* thr = (prThread*)data;
	
	uint8_t forceclose = 0;
	ssize_t byteRecv;
	
	while(thr->sock->csock != INVALID_SOCKET && forceclose == 0) {
		char buffer[10240] = "";
		byteRecv = recv(thr->sock->csock, buffer, 10240, 0);
		if(byteRecv != SOCKET_ERROR && byteRecv != 0) {
			if(strlen(buffer) > 0 ) printDebug("%s",buffer);
		}
		else
			forceclose = 1;
	}
	if(thr->sock->csock != INVALID_SOCKET)
		close(thr->sock->csock);
	destroyCliThread(thr);
	return (void*)0;
}
