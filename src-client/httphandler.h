#ifndef __HTTP_HANDLER__
#define __HTTP_HANDLER__

#include "common.h"

void handleURL(const char* url);
uint8_t generateHTTPHeader(char* httpheader, uint8_t type, char* host, char* path);
uint8_t getPage(char* host, char* header);
uint8_t checkURLValidity(const char* url);

#endif
