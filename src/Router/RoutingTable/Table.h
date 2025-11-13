#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../utils.h"

typedef struct {
	char CIDR[4];
	unsigned int port;
} Gateway;

class RouteNode{
	public:
		unsigned char *CIDR;
		unsigned int starting_point;
		unsigned int ending_point;

		unsigned char *gateway_ip;
		unsigned int gateway_port;

		void print(){

			printf("Información sobre nodo: \n");
			printf("CIDR: (%d, %d, %d, %d)\n", *(CIDR), *(CIDR+1) , *(CIDR+2),*(CIDR+3));
			printf("Rango de puertos: [%u ; %u] \n", starting_point, ending_point);
			printf("Destino: (%d, %d, %d, %d):%u\n", *(gateway_ip), *(gateway_ip+1) , *(gateway_ip+2),*(gateway_ip+3), gateway_port);

		}

		RouteNode* next;
};


class RouteTable{
	public:
		RouteNode *phead;
		unsigned long long size;

		void print(){

			printf("Imprimiendo información de tabla...\n");
			printf("Tamaño de la tabla: %lld\n", size);


			RouteNode *current = phead;

			printf("Cabeza alojada en ");
			printf("%p\n", phead);

			for (unsigned long long i = 0; i<size; i++){
				printf("\n");				
				printf("Revisando nodo %p\n", current);
				
				current->print();

				printf("\n");


				current = current->next;				
			}
		}

		RouteNode *lookup(unsigned char *ip, unsigned int port){
			
			
			for (unsigned long long i = 0; (i < size); i++){

				for(int j = 0; j<4; j++){
					if(*(phead->CIDR+j) != *(ip+j)){
						goto end;
					}
				}

				if((phead->starting_point <= port) && (port <= phead->ending_point)){
					RouteNode* ret = phead;
					phead = phead->next;
					return ret;
				}
				end:
					phead = phead->next;				
					continue;
			}

			return NULL;
		}
};


RouteTable *makeTable(FILE *f);