#include "../../config.h"
#include <netinet/in.h>





typedef struct{
	unsigned char direccion[4];
	unsigned int puerto;
	unsigned char ttl;
	char raw_msg[MSG_SIZE+1]; //+1 accounts for extra \0 at end of string. Just for quick string support
	sockaddr_in addr;	
} legacyPacket;


class Packet{
	
	public:
	sockaddr_in addr;

	//STRAIGHT FROM LEGACY PACKET FOR COMPATIBILITY PURPOSES ONLY. IF TIME, THIS WILL BE DEPRECATED
	unsigned char direccion[4];
	unsigned int puerto;
	unsigned char ttl;
	char raw_msg[MSG_SIZE+1]; //+1 accounts for extra \0 at end of string. Just for quick string support
	sockaddr_in addr;

	Packet(int socket){
		int len = sizeof(addr);
		int status = recvfrom(socket, rawPacket, HEADER_SIZE+MSG_SIZE, 0, (struct sockaddr*)&addr, (socklen_t*)&len);

		if(status<0){
			perror("Error while reading");
			exit(1);
		}


	}
	
	private:
		void* rawPacket = malloc(HEADER_SIZE + MSG_SIZE);
};