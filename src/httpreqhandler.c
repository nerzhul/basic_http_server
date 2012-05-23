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
			if(preg_match(buffer,"^GET[ ](.+)$") == 0) {
				uint16_t retcode = handleGetRequest(buffer,sock);
				forgeHeader(retcode,sock,10);
				return retcode;
			}
			else if(preg_match(buffer,"^POST[ ](.+)$") == 0)
				handlePostRequest(buffer);
			else if(preg_match(buffer,"^PUT[ ](.+)$") == 0)
				handlePutRequest(buffer);
			else {
				forgeHeader(HTTP_ERROR_NOT_IMPLEMENTED,sock,10);
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
	char filename[256];
	bzero(filename,256);
	
	char** matches = malloc(1*sizeof(char*));
	size_t reccount = preg_split(req,"(^GET[ ](.+)[ ])",matches);
	
	if(reccount > 1)
		return HTTP_ERROR_SERVER_ERROR;

	if(strlen(matches[0]) < MIN_CHAR_GET_REQ)
		return HTTP_ERROR_SERVER_ERROR;
	
	if(matches[0][4] != '/')
		return HTTP_ERROR_SERVER_ERROR;
		
	strncpy (filename, &(matches[0])[5], strlen(matches[0])-1);
	printDebug("filename %s",filename);
	FILE* file = fopen(filename,"r");
	if(file == NULL) {
		forgeHeader(HTTP_ERROR_NOT_FOUND,sock,0);
		return HTTP_ERROR_NOT_FOUND;
	}
	
	forgeHeader(HTTP_OK,sock,10);
	return HTTP_OK;
}

int8_t handlePostRequest(char* req) {
	
	return 0;
}

int8_t handlePutRequest(char* req) {
	
	return 0;
}

void forgeHeader(uint16_t code, cSocket* sock, size_t clength) {
	char retcode[512];
	bzero(retcode,512);
	switch(code) {
		case 200: {
			char tmpstr[128];
			bzero(tmpstr,128);
			strcpy(retcode,"HTTP/1.1 200 OK\n");
			strcat(retcode,"Server: esgi-http-server\n");
			strcat(retcode,"Connection: Keep-Alive\n");
			sprintf(tmpstr,"Content-Length: %d\n", clength);
			strcat(retcode,tmpstr);
			strcat(retcode,"\n");
			break;
		}
		case 401:
			break;
		case 404: {
			char tmpstr[128];
			bzero(tmpstr,128);
			strcpy(retcode,"HTTP/1.1 404 Not Found\n");
			strcat(retcode,"Server: esgi-http-server\n");
			strcat(retcode,"Connection: Keep-Alive\n");
			sprintf(tmpstr,"Content-Length: %d\n", strlen("<h1>Not found</h1>\n"));
			strcat(retcode,"\n");
			strcat(retcode,"<h1>Not found</h1>\n");
			break;
		}
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
