#ifndef __PRTHREAD_H__
#define __PRTHREAD_H__

#include <pthread.h>
#include "clientsocket.h"

typedef struct prThread prThread;

struct prThread {
	pthread_t* thread;
	cSocket* sock;
	prThread* next;
};

// for ipv4
prThread* thrList4;

void initThreadlist();
void createCliThread(SOCKET csock, SOCKADDR_IN csin, socklen_t recsize);

#endif
