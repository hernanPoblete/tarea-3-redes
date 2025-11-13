#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include "utils.h"
void force_quit(char* msg, int status){
	perror(msg);
	exit(status);
}

unsigned long char_arr_to_ip_long(unsigned char* arr){
	char *parsed = (char*)malloc(15);

	unsigned long ret = 0;
	sprintf(parsed, "%hhu.%hhu.%hhu.%hhu", *(arr+0), *(arr+1), *(arr+2), *(arr+3));
	
	inet_pton(AF_INET, parsed, &ret);
	return ret;
}

char* char_arr_to_ip_str(unsigned char* arr){
	char *parsed = (char*)malloc(15);

	sprintf(parsed, "%hhu.%hhu.%hhu.%hhu", *(arr+0), *(arr+1), *(arr+2), *(arr+3));
	
	return parsed;	
}

char* long_addr_to_ip_str(unsigned long laddr){
	printf("ld", laddr);
	return "";
}