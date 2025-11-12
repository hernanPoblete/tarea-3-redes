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
	r.routing_table->print();


	return 0;
}