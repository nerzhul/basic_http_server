#include "options.h"

void initExecOptions() {
	daemonize = 0;
	chroot_path = malloc((strlen("/var/www/")+1)*sizeof(char));
	strcpy(chroot_path,"/var/www/");
}

uint8_t readExecOptions(int argc, char** argv) {
	if(argc == 1)
		return 0;
	
	int i;
	for(i=1;i<argc;i++) {
		if(strcmp(argv[i],"-D") == 0)
			daemonize = 1;
		else {
			return 1;
		}
	}
	
	
	return 0;
}
