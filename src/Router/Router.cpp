#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Router.h"

Router makeRouter(char* ip, int port, char* filename){

	//Aloja en memoria los datos del router
	Router *rp = (Router*) malloc(sizeof(Router));

	Router r = *rp;
	FILE* tableFile = fopen(filename, "r");

	r.routing_table = makeTable(tableFile);

	fclose(tableFile);

	unsigned long addr = 0;
	inet_pton(AF_INET, ip, &addr);

	r.address.sin_family = AF_INET;
	r.address.sin_addr.s_addr = addr;
	r.address.sin_port = htons(port);

	r.sock_num = socket(AF_INET, SOCK_DGRAM, 0);
	r.opt = 1;

	if(r.sock_num < 0){
		perror("Socket creation failed!");
		exit(r.sock_num);
	}


	//Binding

	if(setsockopt(r.sock_num, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &r.opt, sizeof(r.opt))){
		perror("setsockopt");
		exit(EXIT_FAILURE);
	};

	if(bind(r.sock_num, (struct sockaddr*)&r.address, sizeof(r.address))<0){
		perror("bind failed");
		exit(1);
	}


	return r;
};

