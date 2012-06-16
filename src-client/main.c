#include "httphandler.h"
#include "stdmgmt.h"

int main(int argc, const char** argv)
{
	printSystem("ESGI http-client version %s",VERSION);
	
	if(argc != 2)
	{
		printUsage();
		return 1;
	}
	
	handleURL(argv[1]);
	
	return 0;
}

void printUsage()
{
	printError("Usage http-client <url>");
}
