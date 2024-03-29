#include "httpreqhandler.h"
#include "options.h"

uint16_t handleRequest(char* req, cSocket* sock) {
	if(req == NULL || sock == NULL)
		return -1;
		
	if(strlen(req) == 0)
		return -1;

	// init buffer for requests
	char buffer[1024];
	bzero(buffer,1024);
	uint16_t offset = 0;
	uint16_t bufferoffset = 0;
	
	while(offset < strlen(req) && req[offset] != '\0') {
		// copy request to buffer
		buffer[bufferoffset] = req[offset];
		
		// if string is ended
		if(req[offset] == '\n' || req[offset] == '\0') {
			buffer[bufferoffset] = '\0';
			// check what request is it and create header for request
			if(preg_match(buffer,"^GET[ ](.+)$") == 0) {
				uint16_t retcode = handleGetRequest(buffer,sock);
				if(retcode != HTTP_OK) forgeHeader(retcode,sock,"");
				return retcode;
			}
			else if(preg_match(buffer,"^POST[ ](.+)$") == 0) {
				uint16_t retcode = handlePostRequest(buffer,sock);
				if(retcode != HTTP_OK) forgeHeader(retcode,sock,"");
				return retcode;
			}
			else if(preg_match(buffer,"^PUT[ ](.+)$") == 0) {
				uint16_t retcode = handlePutRequest(buffer,sock);
				if(retcode != HTTP_OK) forgeHeader(retcode,sock,"");
				return retcode;
			}
			else {
				forgeHeader(HTTP_ERROR_NOT_IMPLEMENTED,sock,"");
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
	char* filename = NULL;
	
	char** matches = malloc(1*sizeof(char*));
	size_t reccount = preg_split(req,"(^GET[ ](.+)[ ])",matches);
	
	if(reccount > 1)
		return HTTP_ERROR_SERVER_ERROR;

	if(strlen(matches[0]) < MIN_CHAR_GET_REQ)
		return HTTP_ERROR_SERVER_ERROR;
	
	if(matches[0][4] != '/')
		return HTTP_ERROR_SERVER_ERROR;
	
	filename = (char*)malloc((strlen(chroot_path)+strlen(matches[0])-6+1)*sizeof(char));
	strcpy(filename,chroot_path);
	strncat(filename, &(matches[0])[5], strlen(matches[0])-6);
	strcat(filename,"\0");
	
	// free useless var from memory
	free(matches[0]);
	free(matches);
	
	FILE* file = fopen(filename,"r+");
	if(file == NULL) {
		forgeHeader(HTTP_ERROR_NOT_FOUND,sock,"");
		free(filename);
		return HTTP_ERROR_NOT_FOUND;
	}
	
	char rchar;
	uint64_t fileoffset = 0;
	
	// First pass to calculate filesize
	do {
		rchar = fgetc(file);
		fileoffset++;
	} while(rchar != EOF);
	
	// come back to the first offset
	rewind(file);
	
	// allocate the right memory size (optimize memory)
	char* content = malloc((fileoffset+1)*sizeof(char));
	fileoffset = 0;
	// copy file to content buffer
	do {
		rchar = fgetc(file);
		if(rchar != EOF) {
			content[fileoffset] = rchar;
			fileoffset++;
		}
	} while(rchar != EOF);
	
	content[fileoffset] = '\0';
	forgeHeader(HTTP_OK,sock,content);
	// proper close & memory free
	fclose(file);
	
	free(filename);
	//free(content); // memory bug for now :s
	return HTTP_OK;
}

int8_t handlePostRequest(char* req, cSocket* sock) {
	char* filename = NULL;
	
	char** matches = malloc(1*sizeof(char*));
	size_t reccount = preg_split(req,"(^POST[ ](.+)[ ])",matches);
	
	if(reccount > 1)
		return HTTP_ERROR_SERVER_ERROR;

	if(strlen(matches[0]) < MIN_CHAR_GET_REQ)
		return HTTP_ERROR_SERVER_ERROR;
	
	if(matches[0][4] != '/')
		return HTTP_ERROR_SERVER_ERROR;
	
	filename = (char*)malloc((strlen(chroot_path)+strlen(matches[0])-6+1)*sizeof(char));
	strcpy(filename,chroot_path);
	strncat(filename, &(matches[0])[5], strlen(matches[0])-6);
	strcat(filename,"\0");
	
	// free useless var from memory
	free(matches[0]);
	free(matches);
	
	FILE* file = fopen(filename,"r+");
	if(file == NULL) {
		forgeHeader(HTTP_ERROR_NOT_FOUND,sock,"");
		free(filename);
		return HTTP_ERROR_NOT_FOUND;
	}
	
	char rchar;
	uint64_t fileoffset = 0;
	
	// First pass to calculate filesize
	do {
		rchar = fgetc(file);
		fileoffset++;
	} while(rchar != EOF);
	
	// come back to the first offset
	rewind(file);
	
	// allocate the right memory size (optimize memory)
	char* content = malloc((fileoffset+1)*sizeof(char));
	fileoffset = 0;
	// copy file to content buffer
	do {
		rchar = fgetc(file);
		if(rchar != EOF) {
			content[fileoffset] = rchar;
			fileoffset++;
		}
	} while(rchar != EOF);
	
	content[fileoffset] = '\0';
	forgeHeader(HTTP_OK,sock,content);
	// proper close & memory free
	fclose(file);
	
	free(filename);
	//free(content); // memory bug for now :s
	return HTTP_OK;
}

int8_t handlePutRequest(char* req, cSocket* sock) {
	char* filename = NULL;
	
	char** matches = malloc(1*sizeof(char*));
	size_t reccount = preg_split(req,"(^PUT[ ](.+)[ ])",matches);
	
	if(reccount > 1)
		return HTTP_ERROR_SERVER_ERROR;

	if(strlen(matches[0]) < MIN_CHAR_GET_REQ)
		return HTTP_ERROR_SERVER_ERROR;
	
	if(matches[0][4] != '/')
		return HTTP_ERROR_SERVER_ERROR;
	
	filename = (char*)malloc((strlen(chroot_path)+strlen(matches[0])-6+1)*sizeof(char));
	strcpy(filename,chroot_path);
	strncat(filename, &(matches[0])[5], strlen(matches[0])-6);
	strcat(filename,"\0");
	
	// free useless var from memory
	free(matches[0]);
	free(matches);
	
	FILE* file = fopen(filename,"r+");
	if(file == NULL) {
		forgeHeader(HTTP_ERROR_NOT_FOUND,sock,"");
		free(filename);
		return HTTP_ERROR_NOT_FOUND;
	}
	
	char rchar;
	uint64_t fileoffset = 0;
	
	// First pass to calculate filesize
	do {
		rchar = fgetc(file);
		fileoffset++;
	} while(rchar != EOF);
	
	// come back to the first offset
	rewind(file);
	
	// allocate the right memory size (optimize memory)
	char* content = malloc((fileoffset+1)*sizeof(char));
	fileoffset = 0;
	// copy file to content buffer
	do {
		rchar = fgetc(file);
		if(rchar != EOF) {
			content[fileoffset] = rchar;
			fileoffset++;
		}
	} while(rchar != EOF);
	
	content[fileoffset] = '\0';
	forgeHeader(HTTP_OK,sock,content);
	// proper close & memory free
	fclose(file);
	
	free(filename);
	//free(content); // memory bug for now :s
	return HTTP_OK;
}

void forgeHeader(uint16_t code, cSocket* sock, char* content) {
	// forge header
	char* retcontent = malloc((128+(content != NULL ? strlen(content) : 0))*sizeof(char));
	// check HTTP code and create header for each
	switch(code) {
		case 200: {
			char tmpstr[64];
			bzero(tmpstr,64);
			strcpy(retcontent,"HTTP/1.1 200 OK\n");
			strcat(retcontent,"Server: esgi-http-server\n");
			strcat(retcontent,"Connection: Keep-Alive\n");
			sprintf(tmpstr,"Content-Length: %d\n", strlen(content));
			strcat(retcontent,tmpstr);
			strcat(retcontent,"\n");
			strcat(retcontent,content);
			break;
		}
		case 401:
			break;
		case 404: {
			char tmpstr[64];
			bzero(tmpstr,64);
			strcpy(retcontent,"HTTP/1.1 404 Not Found\n");
			strcat(retcontent,"Server: esgi-http-server\n");
			strcat(retcontent,"Connection: Keep-Alive\n");
			sprintf(tmpstr,"Content-Length: %d\n", strlen("<h1>Not found</h1>\n"));
			strcat(retcontent,tmpstr);
			strcat(retcontent,"\n");
			strcat(retcontent,"<h1>Not found</h1>\n");
			break;
		}
		case 500: {
			char tmpstr[64];
			bzero(tmpstr,64);
			strcpy(retcontent,"HTTP/1.1 500 Internal Error\n");
			strcat(retcontent,"Server: esgi-http-server\n");
			strcat(retcontent,"Connection: Keep-Alive\n");
			sprintf(tmpstr,"Content-Length: %d\n", strlen("<h1>Internal Server Error</h1>\n"));
			strcat(retcontent,tmpstr);
			strcat(retcontent,"\n");
			strcat(retcontent,"<h1>Internal Server Error</h1>\n");
			break;
		}
		case 501: {
			char tmpstr[64];
			bzero(tmpstr,64);
			strcpy(retcontent,"HTTP/1.1 501 Not Implemented\n");
			strcat(retcontent,"Server: esgi-http-server\n");
			strcat(retcontent,"Connection: Keep-Alive\n");
			sprintf(tmpstr,"Content-Length: %d\n", strlen("<h1>Not Implemented</h1>\n"));
			strcat(retcontent,tmpstr);
			strcat(retcontent,"\n");
			strcat(retcontent,"<h1>Not Implemented</h1>\n");
			break;
		}
	}
	
	if(sock->http_reply != NULL) {
		free(sock->http_reply);
		sock->http_reply = NULL;
	}
	
	// copy header into socket buffer for reply (at the start)
	sock->http_reply = (char*)malloc((strlen(retcontent)+1)*sizeof(char));
	strcpy(sock->http_reply,retcontent);
}
