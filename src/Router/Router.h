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
			Packet* ppack = (Packet*)malloc(sizeof(Packet));

			//int status = read(sock_num, ppack, sizeof(Packet));

			int len = sizeof(ppack->addr);
			int status = recvfrom(sock_num, ppack, HEADER_SIZE+MSG_SIZE, 0, (struct sockaddr*)&ppack->addr, (socklen_t*)&len);

			if(status<0){
				perror("Error while reading");
				exit(1);
			}
			ppack->raw_msg[status-HEADER_SIZE] = '\0';
			return ppack;
		}

		void mainLoop(){
			Packet *recv = readMsg();
			if(address.sin_port != recv->puerto || address.sin_addr.s_addr != char_arr_to_ip_long(recv->direccion)){
				RouteNode *node = routing_table->lookup(recv->direccion, recv->puerto);

				if(node == NULL){
					printf("No hay ruta para llegar hasta %s desde la dirección %s\n", char_arr_to_ip_str(recv->direccion), long_addr_to_ip_str(recv->addr.sin_addr.s_addr)); //TODO: Añadir dirección
				}else{
					printf("TODO: Realizar salto\n");
				}
			}else{
				printf("%s\n", recv->raw_msg);
			}


			
			mainLoop();
		}

};

Router makeRouter(char* ip, int port, char* filename);
void destroyRouter(Router* r);