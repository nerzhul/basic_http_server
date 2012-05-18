#ifndef __SERVER_SOCKET_H__
#define __SERVER_SOCKET_H__

#include "sock_def.h"
    
int ssock_err;
SOCKET ssock;
SOCKADDR_IN ssin;

#define HTTP_PORT 80

int8_t initServerSocket();
int8_t ssocketListen();
int8_t handleClients();
void ssocketClose();
#endif
