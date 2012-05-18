#ifndef __CLIENT_SOCKET_H__
#define __CLIENT_SOCKET_H__

#include "sock_def.h"

typedef struct cSocket cSocket;

struct cSocket {
	SOCKET csock;
    SOCKADDR_IN csin;
    socklen_t recsize;
};

void* clientHandler(void* data);

#endif
