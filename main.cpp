#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;

#include "src/Router/Router.h"
#include "src/Router/RoutingTable/Table.h"


int main(int argc, char *argv[]){
	if(argc != 4) {
		printf("Usage: T3 <IP Address> <Listen Port> <Route for Route Table>\n");
		exit(1);
	}

	FILE *f = fopen(argv[3], "r");

	RouteTable table = makeTable(f);

	table.print();
/* 	RouteTable table(f);
	table.print(); */
	return 0;
}