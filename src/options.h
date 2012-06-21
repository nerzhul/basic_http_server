#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#include "common.h"

char cur_read_opt;
uint8_t daemonize;
char* chroot_path;

void initExecOptions();
uint8_t readExecOptions(int argc, char** argv);

#endif
