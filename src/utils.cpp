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
	unsigned char * ret = (unsigned char*) malloc(sizeof(unsigned long));
	
	for(int i = 0; i<sizeof(unsigned long); i++){
		unsigned long mask = (1<<8) -1;

		*(ret+i) =(laddr&(mask<<(i<<3)))>>(i<<3); //Mateu estaría orgulloso :3
		
	}


	return char_arr_to_ip_str(ret);
}