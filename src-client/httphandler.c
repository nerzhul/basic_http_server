#include "httphandler.h"
#include "stdmgmt.h"

void handleURL(const char* url)
{
	uint8_t err = checkURLValidity(url);
	switch(err)
	{
		case 0: printSuccess("URL is valid !"); break;
		case 1: printError("URL is invalid !"); break;
		case 2: printError("Protocol is invalid !"); break;
		default: printError("Unhandled return code"); break;
	}
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
