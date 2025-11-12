#include "../config.h"
#include "RoutingTable/Table.h"

class Router{
	public:
		struct sockaddr_in address;
		RouteTable *routing_table;
		int sock_num;
		int opt;

		
		void readMsg(char **dest){
			int status = read(sock_num, *dest, PACKET_SIZE);
			printf("Status: %i\n", status);
			if(status<0){
				perror("Error while reading");
				exit(1);
			}


		}
};


char* ip_to_bytes(char *ip);
Router makeRouter(char* ip, int port, char* filename);
void destroyRouter(Router* r);