#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
void force_quit(char* msg, int status){
	perror(msg);
	exit(status);
}