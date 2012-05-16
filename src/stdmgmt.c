#include "stdmgmt.h"

void printSystem(const char* str, ...) {
	char buffer[1024];
	va_list ap;
	
	setOutputColor(COLOR_ORANGE);
	
    va_start(ap, str);
    vsprintf(buffer, str, ap);
    fprintf(stdout,buffer);
	va_end(ap);
	
	unsetOutputColor();
	
    fprintf(stdout, "\n");
	fflush(stdout);
}

void printDebug(const char* str, ...) {
	char buffer[1024];
	va_list ap;
	
	setOutputColor(COLOR_BLUE);
	
    va_start(ap, str);
    vsprintf(buffer, str, ap);
    fprintf(stdout,buffer);
	va_end(ap);
	
	unsetOutputColor();
	
    fprintf(stdout, "\n");
	fflush(stdout);
}

void printError(const char* str, ...) {
	char buffer[1024];
	va_list ap;
	
	setOutputColor(COLOR_RED);
	
    va_start(ap, str);
    vsprintf(buffer, str, ap);
    fprintf(stdout,buffer);
	va_end(ap);
	
	unsetOutputColor();
	
    fprintf(stdout, "\n");
	fflush(stdout);
}

void setOutputColor(int color) {
    fprintf(stdout, "\x1b[%dm",color);
}

void unsetOutputColor() {
	fprintf(stdout, "\x1b[0m");
}
