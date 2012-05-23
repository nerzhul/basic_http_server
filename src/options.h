#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#include "common.h"

uint8_t daemonize;
char* chroot_path;

void initExecOptions();
uint8_t readExecOptions(int argc, char** argv);

#endif
