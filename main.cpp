#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;

#include "src/Router/Router.h"


int main(int argc, char *argv[]){
	if(argc != 4) {
		printf("Usage: T3 <IP Address> <Listen Port> <Route for Route Table>\n");
		exit(1);
	}

	FILE *f = fopen(argv[3], "r");

	Router r = makeRouter(argv[1], atoi(argv[2]), argv[3]);

	

	unsigned char* ip_mock = (unsigned char *) malloc(4);
	*ip_mock = 127;
	*(ip_mock+1) = 0;
	*(ip_mock+2) = 0;
	*(ip_mock+3) = 1;

	unsigned int port_mock = 5500;

	RouteNode* node;


	return 0;
}