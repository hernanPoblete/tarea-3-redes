#include "Table.h"
#include <stdlib.h>
#include "../../utils.h"

RouteNode* makePNode(FILE *ftable){
	RouteNode *pnode = (RouteNode*) malloc(sizeof(RouteNode));

	unsigned char *CIDR = (unsigned char*) malloc(4);



	unsigned int starting_point;
	unsigned int ending_point;

	unsigned char *gateway_ip = (unsigned char*) malloc(4);;
	unsigned int gateway_port;

	
	int status = fscanf(ftable, "%hhu.%hhu.%hhu.%hhu %u %u %hhu.%hhu.%hhu.%hhu %u\n", 
		CIDR, CIDR+1, CIDR+2, CIDR+3,

		&starting_point, &ending_point,

		gateway_ip, gateway_ip+1,gateway_ip+2,gateway_ip+3,
		&gateway_port
	);

	if (status<=0){
		return NULL;
	}
	
	pnode->CIDR = CIDR;
	pnode->starting_point = starting_point;
	pnode->ending_point = ending_point;
	pnode->gateway_ip = gateway_ip;
	pnode->gateway_port = gateway_port;


	pnode->next = makePNode(ftable);
	return pnode;
}

RouteTable makeTable(FILE *f){
	RouteTable *ptable = (RouteTable*) malloc(sizeof(RouteTable));
	
	ptable->phead = makePNode(f);

	return *ptable;
};