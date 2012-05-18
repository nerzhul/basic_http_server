#include <strings.h>
#include "httpreqhandler.h"

uint16_t handleRequest(char* req, cSocket* sock) {
	if(req == NULL || sock == NULL)
		return -1;
		
	if(strlen(req) == 0)
		return -1;

	char buffer[1024];
	bzero(buffer,1024);
	uint16_t offset = 0;
	uint16_t bufferoffset = 0;
	
	while(offset < strlen(req) && req[offset] != '\0') {
		buffer[bufferoffset] = req[offset];
		if(req[offset] == '\n' || req[offset] == '\0') {
			buffer[bufferoffset] = '\0';
			if(preg_match(buffer,"^GET[ ](.+)$") == 0)
				return handleGetRequest(buffer,sock);
			else if(preg_match(buffer,"^POST[ ](.+)$") == 0)
				handlePostRequest(buffer);
			else if(preg_match(buffer,"^PUT[ ](.+)$") == 0)
				handlePutRequest(buffer);
			else {
				forgeHeader(HTTP_ERROR_NOT_IMPLEMENTED,sock);
				return HTTP_ERROR_NOT_IMPLEMENTED;
			}
			bzero(buffer,1024);
			bufferoffset = 0;
		}
		else
			bufferoffset++;
		
		offset++;
	}
	
	return 0;
}

uint16_t handleGetRequest(char* req, cSocket* sock) {
	forgeHeader(200,sock);
	return 0;
}

int8_t handlePostRequest(char* req) {
	
	return 0;
}

int8_t handlePutRequest(char* req) {
	
	return 0;
}

void forgeHeader(uint16_t code, cSocket* sock) {
	char retcode[512];
	bzero(retcode,512);
	switch(code) {
		case 200: {
			char tmpstr[128];
			bzero(tmpstr,128);
			char tmpstr2[128];
			bzero(tmpstr2,128);
			strcpy(retcode,"HTTP/1.1 200 OK\n");
			strcat(retcode,"Server: esgi-http-server\n");
			strcat(retcode,"Connection: Keep-Alive\n");
			
			// @ TODO: change content
			strcpy(tmpstr,"<h1>Hello world</h1><img src=\"/toto.jpg\"/>\n\0");
			sprintf(tmpstr2, "Content-Length: %d\n", strlen(tmpstr));
			
			strcat(retcode,tmpstr2);
			strcat(retcode,"\n");
			strcat(retcode,tmpstr);
			break;
		}
		case 401:
			break;
		case 404:
			strcpy(retcode,"HTTP/1.1 404 Not Found\n");
			strcat(retcode,"Server: esgi-http-server\n\n");
			strcat(retcode,"<h1>Not found</h1>\n");
			break;
		case 500:
			break;
		case 501:
			break;
		}
	
	if(sock->http_reply != NULL) {
		free(sock->http_reply);
		sock->http_reply = NULL;
	}
	
	sock->http_reply = (char*)malloc((strlen(retcode)+1)*sizeof(char));
	strcpy(sock->http_reply,retcode);
}
