#include "../../config.h"
#include <netinet/in.h>





typedef struct{
	unsigned char direccion[4];
	unsigned int puerto;
	char raw_msg[MSG_SIZE+1]; //+1 accounts for extra \0 at end of string. Just for quick string support
	sockaddr_in addr;	
} Packet;

