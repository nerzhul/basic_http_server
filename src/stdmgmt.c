#include <regex.h>
#include <strings.h>
#include "stdmgmt.h"

void printSystem(const char* str, ...) {
	char buffer[1024];
	bzero(buffer,1024);
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
	bzero(buffer,1024);
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
	bzero(buffer,1024);
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

int8_t preg_match(char* str, char* rgx) {
	int err;
	regex_t preg;
	err = regcomp (&preg, rgx, REG_NOSUB | REG_EXTENDED);
	if (err == 0)
	{
		int match;
		match = regexec (&preg, str, 0, NULL, 0);
		regfree (&preg);
		if (match == 0)
			return 0;
	}
	return -1;
}

size_t preg_split(char* str, char* rgx,char** matches) {
	int err;
	regex_t preg;
	err = regcomp (&preg, rgx, REG_EXTENDED);
	if (err == 0)
	{
		int match;
		size_t nmatch = 0;
		regmatch_t* pmatch = NULL;

		nmatch = preg.re_nsub;
		pmatch = malloc (sizeof (*pmatch) * nmatch);
		if (pmatch)
		{
			match = regexec (&preg, str, nmatch, pmatch, 0);
			regfree (&preg);
			if (match == 0)
			{
				int i;
				for(i=0;i<nmatch;i++) {
					char *site = NULL;
					int start = pmatch[i].rm_so;
					int end = pmatch[i].rm_eo;
					size_t size = end - start;
					   
					site = malloc (sizeof (*site) * (size + 1));
					printDebug("start %d, end %d\n",start,end);
					if (site)
					{
						// @ TODO verify copy
						strncpy (site, &str[start], size);
						site[size] = '\0';
						printDebug("%s", site);
						free (site);
					}
				}
				
			}
		}
	}
	return 0;
}
