#include "../config.h"
#include "RoutingTable/Table.h"
#include "Packets/Defragmentator.h"
#include <bits/stdc++.h>
using namespace std;
class Router{
	public:
		struct sockaddr_in address;
		RouteTable *routing_table;
		int sock_num;
		int opt;

		void sendPacket(Packet pack, RouteNode* node){
			if(node == NULL){
				printf("No hay ruta para llegar hasta %s desde la dirección %s\n", char_arr_to_ip_str(pack.direccion), long_addr_to_ip_str(pack.addr.sin_addr.s_addr));
			}else{
				printf("Redirigiendo paquete en dirección de memoria %p con destino final %s:%u, desde %s:%u hacia %s:%u\n", 
					&pack, 
					char_arr_to_ip_str(pack.direccion), pack.puerto, 
					long_addr_to_ip_str(address.sin_addr.s_addr), htons(address.sin_port),
					char_arr_to_ip_str(node->gateway_ip), node->gateway_port
				);

				struct sockaddr_in out;

				out.sin_family = AF_INET;
				out.sin_port = htons(node->gateway_port);

				unsigned long ip = 0;
				inet_pton(AF_INET, char_arr_to_ip_str(node->gateway_ip) ,&ip);

				out.sin_addr.s_addr = ip;
				pack.ttl-=1;


				sendto(sock_num, pack.asBuf(), HEADER_SIZE+pack.msg_length, 0, (struct sockaddr*)&out, (socklen_t)sizeof(out));
			}	
		}

		void sendPacketByFragments(Packet pack, RouteNode* node){
			unsigned int frag_amount = ceil(((double) pack.msg_length)/((double) node->MTU));

			unsigned int remaining = pack.msg_length;
			char* text = pack.raw_msg;
			unsigned int offset = 0;


			for(int i = 0; i < frag_amount; i++){
				unsigned int frag_size = min(node->MTU, remaining);
				unsigned char new_flag = (pack.flag)|| (frag_amount!=i+1);

				char fragContent[frag_size];

				snprintf(fragContent, frag_size+1, "%s", text);
				
				Packet toSend(pack.addr, pack.direccion, pack.puerto, pack.ttl, pack.ID, offset, new_flag, frag_size, fragContent);

				sendPacket(toSend, node);


				text+=frag_size;
				remaining -= frag_size;
				offset += frag_size;
			}
		}

		void mainLoop(){
			Packet pack(sock_num);

			if(pack.ttl == 0){
				printf("Se recibió paquete desde %s:%u con TTL 0\n" , long_addr_to_ip_str(pack.addr.sin_addr.s_addr), htons(pack.addr.sin_port));
			} else if(address.sin_port != htons(pack.puerto) || address.sin_addr.s_addr != char_arr_to_ip_long(pack.direccion)){
				RouteNode *node = routing_table->lookup(pack.direccion, pack.puerto);
				sendPacketByFragments(pack, node);
			}else{


				/**
				 * Acá va toda la lógica de defragmentación de paquetes...
				 */
				printf("%s\n", pack.raw_msg);
			}


			
			mainLoop();
		}

	private:
		Defragmentator defrag;
};

Router makeRouter(char* ip, int port, char* filename);
void destroyRouter(Router* r);