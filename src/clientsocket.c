#include "clientsocket.h"
#include "prthread.h"

void* clientHandler(void* data) {
	prThread* thr = (prThread*)data;
	
	ssize_t byteRecv;
	char buffer[10240];
	while((byteRecv = recv(thr->sock, buffer, 10240, 0)) != SOCKET_ERROR) {
	}
	destroyCliThread(thr);
}
