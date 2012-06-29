#include <pthread.h>
#include <stdlib.h>
#include "prthread.h"

void initThreadlist() {
	thrList4 = NULL;
}

void createCliThread(SOCKET csock, SOCKADDR_IN csin, socklen_t recsize) {
	// Create a client thread
	prThread* cursor = thrList4;
	prThread* thr = (prThread*)malloc(sizeof(prThread));
	cSocket* sock = (prThread*)malloc(sizeof(cSocket));
	sock->csock = csock;
	sock->csin = csin;
	sock->recsize = recsize;
	sock->http_reply = NULL;
	thr->sock = sock;
	thr->next = NULL;
	thr->thread = malloc(sizeof(pthread_t));
	
	// register thread in chained list
	if(cursor == NULL) {
		thrList4 = thr;
	} else {
		while(cursor->next != NULL)
			cursor = cursor->next;
		cursor->next = thr;
	}
	
	// create a thread for this client
	pthread_create(thr->thread, NULL, clientHandler, (void*)thr);
}

void destroyCliThread(prThread* thr) {
	if(thr == NULL)
		return;
	
	if(thr->sock != NULL)
		free(thr->sock);
	
	// remove from chained list
	prThread* cursor = thrList4;
	if(cursor == thr) {
		thrList4 = cursor->next;
	} else {
		while(cursor->next != thr && cursor->next != NULL)
			cursor = cursor->next;
		if(cursor->next != NULL)
			cursor->next = cursor->next->next;
	}
	free((void*)thr);
}
