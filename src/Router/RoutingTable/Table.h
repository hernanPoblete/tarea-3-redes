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

		void print(){

			printf("Imprimiendo información de tabla...\n");


			RouteNode *current = phead;

			printf("Cabeza alojada en ");
			printf("%p\n", phead);

			while(current!=NULL){
				printf("\n");				
				printf("Revisando nodo %p\n", current);
				
				current->print();

				printf("\n");


				current = current->next;				
			}
		}
};


RouteTable makeTable(FILE *f);