#include "../config.h"
#include "RoutingTable/Table.h"
#include "Packets/Packet.h"


class Router{
	public:
		struct sockaddr_in address;
		RouteTable *routing_table;
		int sock_num;
		int opt;


		void mainLoop(){
			Packet recv_full(sock_num);
			Packet* recv = &recv_full;
			
			if(recv->ttl == 0){
				printf("Se recibió paquete desde %s:%u con TTL 0\n" , long_addr_to_ip_str(recv->addr.sin_addr.s_addr), htons(recv->addr.sin_port));
			} else if(address.sin_port != htons(recv->puerto) || address.sin_addr.s_addr != char_arr_to_ip_long(recv->direccion)){
				RouteNode *node = routing_table->lookup(recv->direccion, recv->puerto);

				if(node == NULL){
					printf("No hay ruta para llegar hasta %s desde la dirección %s\n", char_arr_to_ip_str(recv->direccion), long_addr_to_ip_str(recv->addr.sin_addr.s_addr)); //TODO: Añadir dirección
				}else{
					printf("Redirigiendo paquete en dirección de memoria %p con destino final %s:%u, desde %s:%u hacia %s:%u\n", 
						recv, 
						char_arr_to_ip_str(recv->direccion), recv->puerto, 
						long_addr_to_ip_str(address.sin_addr.s_addr), htons(address.sin_port),
						char_arr_to_ip_str(node->gateway_ip), node->gateway_port
					);

					struct sockaddr_in out;

					out.sin_family = AF_INET;
					out.sin_port = htons(node->gateway_port);

					unsigned long ip = 0;
					inet_pton(AF_INET, char_arr_to_ip_str(node->gateway_ip) ,&ip);

					out.sin_addr.s_addr = ip;
					recv->ttl-=1;
					sendto(sock_num, recv, HEADER_SIZE+MSG_SIZE, 0, (struct sockaddr*)&out, (socklen_t)sizeof(out));
				}
			}else{
				printf("%s\n", recv->raw_msg);
			}


			
			mainLoop();
		}

};

Router makeRouter(char* ip, int port, char* filename);
void destroyRouter(Router* r);