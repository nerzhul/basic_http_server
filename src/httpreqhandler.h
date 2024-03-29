#ifndef __HTTP_REQ_HANDLER_H__
#define __HTTP_REQ_HANDLER_H__

#include "common.h"
#include "clientsocket.h"

#define HTTP_OK							200
#define HTTP_ERROR_NOT_FOUND			404
#define HTTP_ERROR_SERVER_ERROR			500
#define HTTP_ERROR_NOT_IMPLEMENTED		501

#define MIN_CHAR_GET_REQ				6

uint16_t handleRequest(char* req, cSocket* sock);
uint16_t handleGetRequest(char* req, cSocket* sock);
int8_t handlePostRequest(char* req, cSocket* sock);
int8_t handlePutRequest(char* req, cSocket* sock);

void forgeHeader(uint16_t code, cSocket* sock, char* content);
#endif
