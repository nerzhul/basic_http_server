#include "httphandler.h"
#include "sock_def.h"
#include "stdmgmt.h"

void handleURL(const char* url)
{
	uint8_t err = checkURLValidity(url);
	if(err > 0)
	{
		switch(err)
		{
			case 1: printError("URL is invalid !"); break;
			case 2: printError("Protocol is invalid !"); break;
			default: printError("Unhandled return code"); break;
		}
		
		return;
	}
	
	char hostbuffer[256];
	bzero(hostbuffer,256);
	char pathbuffer[1024];
	bzero(pathbuffer,1024);

	uint16_t i;
	for(i=7;i<strlen(url);i++)
	{
		if(url[i]=='/')
			break;
		hostbuffer[i-7] = url[i];
	}
	
	uint16_t offset = i;
	for(;i<strlen(url);i++)
		pathbuffer[i-offset] = url[i];

	if(strcmp(pathbuffer,"") == 0)
		strcat(pathbuffer,"/");
	
	char httpheader[10240];
	bzero(httpheader,10240);
	
	generateHTTPHeader(httpheader,1,hostbuffer,pathbuffer);
	getPage(hostbuffer,httpheader);
}

uint8_t getPage(char* host, char* header)
{
	if(host == NULL || strlen(host) == 0 || header == NULL || strlen(header) == 0)
	{
		printError("Fatal Error !! Host or header is invalid !");
		return 1;
	}
	
	struct addrinfo *result;
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sin;
	
	int err = getaddrinfo(host, NULL, NULL,&result);
	if(err != 0 || result == NULL)
	{
		printError("Unable to connect to host:  DNS resolve failure");
		return 2;
	}
	sin.sin_family         = AF_INET;
	sin.sin_port           = htons(80);
	
	sin.sin_addr.s_addr = ((struct sockaddr_in *)(result->ai_addr))->sin_addr.s_addr;
	err = connect(sock, (SOCKADDR*)&sin, sizeof(sin));

	if(err != 0)
	{
		printError("Unable to connect to host: port unreachable");
		return 3;
	}
	
	err = send(sock,header,strlen(header),0);
	if(err != strlen(header))
	{
		printError("Transmission failure with host: connection reset");
		return 4;
	}
	
	char buffer[102400] = "";
	int byteRecv;
	while((byteRecv = recv(sock, buffer, 102400, 0)) != 0)
		printDebug("%s",buffer);
	
	printSuccess("Yeah !");
	
	close(sock);
}

uint8_t generateHTTPHeader(char* httpheader, uint8_t type, char* host, char* path)
{
	if(path == NULL || strlen(path) == 0 || host == NULL || strlen(host) == 0)
		return 1;
		
	switch(type)
	{
		case 1: strcpy(httpheader, "GET "); break;
		case 2: strcpy(httpheader, "POST "); break;
		case 3: strcpy(httpheader, "PUT "); break;
		default: return 1;
	}
	
	strcat(httpheader,path);
	strcat(httpheader," HTTP/1.1\n");
	strcat(httpheader,"Host: ");
	strcat(httpheader,host);
	strcat(httpheader,"\n");
	strcat(httpheader,"User-Agent: ESGI http-client\n");
	strcat(httpheader,"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\n");
	strcat(httpheader,"Accept-Language: en-us,en\n");
	strcat(httpheader,"\n");
	
	return 0;
}

uint8_t checkURLValidity(const char* url)
{
	if(strlen(url) < 8)
		return 1;
		
	char buffer[8];
	bzero(buffer,8);
	
	int i;
	for(i=0;i<7;i++)
		buffer[i] = url[i];
	
	if(strcmp(buffer,"http://") != 0)
		return 2;
	
	return 0;
}
