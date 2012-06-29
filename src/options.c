#include "options.h"

void initExecOptions() {
	daemonize = 0;
	chroot_path = malloc((strlen("/var/www/")+1)*sizeof(char));
	strcpy(chroot_path,"/var/www/");
	cur_read_opt = '\0';
}

uint8_t readExecOptions(int argc, char** argv) {
	// read all options
	int i;
	for(i=1;i<argc;i++) {
		// -D option daemonize the process
		if(strcmp(argv[i],"-D") == 0)
			daemonize = 1;
		// -r (= root) defines the service's root (absolute path)
		else if(strcmp(argv[i],"-r") == 0)
			cur_read_opt = 'r';
		else {
			if(cur_read_opt == 'r')	{
				if(argv[i][0] == '/' && opendir(argv[i]) != NULL) {
					free(chroot_path);
					chroot_path = malloc((strlen(argv[i])+1)*sizeof(char));
					strcpy(chroot_path,argv[i]);
					if(chroot_path[strlen(chroot_path)-1] != '/')
						strcat(chroot_path,"/");
					cur_read_opt = '\0';
				}
				else {
					printError("No absolute path given or path is invalid !");
					return 1;
				}
			}
			else {
				printError("Unknown option %s",argv[i]);
				return 1;
			}
		}
	}
	
	if(cur_read_opt != '\0') {
		printError("An argument is missing for option -%c !",cur_read_opt);
		return 1;
	}
	
	if(opendir(chroot_path) == NULL) {
		printError("Root Path is invalid (%s): verify if exists or the rights",chroot_path);
		return 1;
	}
	
	printSystem("Chrooting to path %s",chroot_path);
	
	return 0;
}
