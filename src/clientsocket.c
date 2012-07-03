#include "clientsocket.h"
#include "prthread.h"

void* clientHandler(void* data) {
	prThread* thr = (prThread*)data;
	
	uint8_t forceclose = 0;
	ssize_t byteRecv;
	
	// While socket is valid and we don't force close
	while(thr->sock->csock != INVALID_SOCKET && forceclose == 0) {
		// put recv datas into a 10ko buffer
		char buffer[10240] = "";
		byteRecv = recv(thr->sock->csock, buffer, 10240, 0);
		// if no socket error
		if(byteRecv != SOCKET_ERROR && byteRecv != 0) {
			if(strlen(buffer) > 0) {
				printDebug("%s",buffer);
				uint16_t err = handleRequest(buffer,thr->sock);
				if(err > 400)
					forceclose = 1;
				if(thr->sock->http_reply && strlen(thr->sock->http_reply) > 0) {
					printDebug("Send reply to client:\n%s",thr->sock->http_reply);
					send(thr->sock->csock,thr->sock->http_reply,strlen(thr->sock->http_reply),0);
				}
			}
		}
		else
			forceclose = 1;
	}
	printDebug("Closing connection with %s:%d (sock %d)",inet_ntoa(thr->sock->csin.sin_addr), htons(thr->sock->csin.sin_port),thr->sock->csock);
	if(thr->sock->csock != INVALID_SOCKET)
		close(thr->sock->csock);
	destroyCliThread(thr);
	return (void*)0;
}
