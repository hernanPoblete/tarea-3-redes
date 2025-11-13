#include "../config.h"
#include "RoutingTable/Table.h"
#include "Packets/Packet.h"


class Router{
	public:
		struct sockaddr_in address;
		RouteTable *routing_table;
		int sock_num;
		int opt;

		
		Packet *readMsg(){
			Packet* ppack = (Packet*)malloc(PACKET_SIZE);

			int status = read(sock_num, ppack, PACKET_SIZE);
			printf("Status: %i\n", status);
			if(status<0){
				perror("Error while reading");
				exit(1);
			}
			ppack->raw_msg[status-HEADER_SIZE] = '\0';
			printf("Paquete alojado en %p\n", ppack);
			printf("\tDirección guardada en: %p\n", ppack->direccion);
			printf("\tPuerto guardado en: %p\n", &(ppack->puerto));
			printf("\tmensaje guardado en: %p\n", ppack->raw_msg);
			return ppack;
		}

};


char* ip_to_bytes(char *ip);
Router makeRouter(char* ip, int port, char* filename);
void destroyRouter(Router* r);