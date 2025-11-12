#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Router.h"

/**
 * Transforma un string de ip en un mapa de 32 bits de dirección ip
 * 
 * @param ip El string conteniente de la dirección
 * @returns Un long con los 32 bits de dirección ip
 */
char* ip_to_bytes(char *ip){
	char *partition[4];
	char map[4] = {0,0,0,0};

	for(int i = 0; i<4; i++){
		partition[i] = (char*) malloc(1);
		partition[i][0] = 0;
	}

	int index = 0;

	for(int i = 0; i<strlen(ip); i++){
		if(ip[i] != '.'){
			strncat(partition[index], ip+i, 1);
		}else{
			index+=1;
		}

	}

	for(int i = 0; i<4; i++){
		map[i] = (atoi(partition[i]));	
	}

	return map;
};


Router makeRouter(char* ip, int port, char* filename){

	//Aloja en memoria los datos del router
	Router *rp = (Router*) malloc(sizeof(Router));

	Router r = *rp;
	r.routing_table = fopen(filename, "r");



	// Trabaja el address del router en estructura

	r.address.sin_family = AF_INET;
	r.address.sin_addr.s_addr = INADDR_ANY; //ip_to_bytes(ip); //TODO: Indagar en esto y en como usar mi IP en vez de esto
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



void destroyRouter(Router* r){

	Router a = *r;
	fclose(a.routing_table);
	//delete r;
}