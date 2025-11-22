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

		// class constructor
		Router();

		Router(char* ip, int port, char* filename){
			FILE* tableFile = fopen(filename, "r");
			routing_table = makeTable(tableFile);
			fclose(tableFile);

			unsigned long addr = 0;
			inet_pton(AF_INET, ip, &addr);

			address.sin_family = AF_INET;
			address.sin_addr.s_addr = addr;
			address.sin_port = htons(port);

			sock_num = socket(AF_INET, SOCK_DGRAM, 0);
			opt = 1;

			if(sock_num < 0){
				perror("Socket creation failed!");
				exit(sock_num);
			}


			//Binding

			if(setsockopt(sock_num, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
				perror("setsockopt");
				exit(EXIT_FAILURE);
			};

			if(bind(sock_num, (struct sockaddr*)&address, sizeof(address))<0){
				perror("bind failed");
				exit(1);
			}
		}

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
			unsigned int frag_amount = (pack.msg_length + node->MTU - HEADER_SIZE - 1)/( node->MTU - HEADER_SIZE);

			unsigned int remaining = pack.msg_length;
			char* text = pack.raw_msg;
			unsigned int offset = pack.offset;

			printf("Enviando %u fragmentos...\n\n\n", frag_amount);
			
			for(int i = 0; i < frag_amount; i++){
				unsigned int content_size = min(node->MTU - HEADER_SIZE, remaining);
				unsigned char new_flag = (pack.flag)||(frag_amount!=i+1);
				
				//cout << "fragsize: " << frag_size << endl;
				char fragContent[content_size+1];
				
				snprintf(fragContent, content_size+1, "%s", text);
				//cout << "adsasdasd" << endl;
				
				Packet toSend(pack.addr, pack.direccion, pack.puerto, pack.ttl, pack.ID, offset, new_flag, content_size, fragContent);

				sendPacket(toSend, node);


				text+=content_size;
				remaining -= content_size;
				offset += content_size;
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

				defrag.addFrag(pack);


				char *potential_full_msg = defrag.join_packets(pack.ID);

				if(potential_full_msg != NULL){
					printf("Mensaje reensamblado!!!!!!!! :3 \n%s\n", potential_full_msg);
				}
			}


			
			mainLoop();
		}

	private:
		Defragmentator defrag;
};

Router makeRouter(char* ip, int port, char* filename);
void destroyRouter(Router* r);