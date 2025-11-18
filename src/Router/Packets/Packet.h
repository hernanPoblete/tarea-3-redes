#include "../../config.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../utils.h"



typedef struct{
	unsigned char direccion[4];
	unsigned int puerto;
	unsigned char ttl;
	char raw_msg[MSG_SIZE+1]; //+1 accounts for extra \0 at end of string. Just for quick string support
	sockaddr_in addr;	
} legacyPacket;


class Packet{
	
	public:

	//STRAIGHT FROM LEGACY PACKET FOR COMPATIBILITY PURPOSES ONLY. IF TIME, THIS WILL BE DEPRECATED	
	sockaddr_in addr;
	unsigned char direccion[4];
	unsigned int puerto;
	unsigned char ttl;
	unsigned int ID;
	unsigned int offset;
	unsigned char flag;
	unsigned int msg_length;

	char raw_msg[MSG_SIZE+1]; //+1 accounts for extra \0 at end of string. Just for quick string support


	Packet(int socket){
		int len = sizeof(addr);
		int status = recvfrom(socket, rawPacket, HEADER_SIZE+MSG_SIZE, 0, (struct sockaddr*)&addr, (socklen_t*)&len);
		if(status<0){
			perror("Error while reading");
			exit(1);
		}

		void *current = rawPacket;
		for(int i = 0; i<4; i++){ // Draws address from the rawPacket
			direccion[i] = *((unsigned char*)current);
			current += 1;	// Recordatorio de que las alertas son solo sugerencias
		}

		sprintf(sdir, "%hhu.%hhu.%hhu.%hhu", direccion[0], direccion[1], direccion[2], direccion[3]);

		puerto = *((unsigned int*)(current)); //Extraer el puerto
		current+=4;
		ttl = *((char*) current); // Recordatorio de que no tendría que estar haciendo este casteo si estuviera en C en vez de CPP
		current += 1;
		ID = *((unsigned int*) current);
		current+=4;
	
		offset = *((unsigned int*) current);
		current+=4;
		msg_length = *((unsigned int*) current);
		current += 4;
		flag = *((unsigned char*) current);
		current+=1;
		snprintf(raw_msg, msg_length+1, "%s", (char*)current);
		
	};


	Packet(	struct sockaddr_in addr_, unsigned char direccion_[4], unsigned int puerto_, unsigned char ttl_, unsigned int ID_, unsigned int offset_, unsigned char flag_, unsigned int msg_length_, char* msg){
		addr = addr_;
		puerto = puerto_;
		ttl = ttl_;
		ID=ID_;
		offset = offset_;
		flag = flag_;
		msg_length = msg_length_;


		for(int i = 0; i<4; i++){
			direccion[i] = direccion_[i];
		}


		snprintf(raw_msg, msg_length+1, "%s", msg);
		sprintf(sdir, "%hhu.%hhu.%hhu.%hhu", direccion[0], direccion[1], direccion[2], direccion[3]);

		void* temp = rawPacket;
		
		
		for(int i = 0; i<4; i++){
			*((unsigned char*)temp) = direccion[i];
			temp += 1;
		}

		*((unsigned int*)temp) = puerto;
		temp+=4;

		*((unsigned char*)temp) = ttl;
		temp+=1;

		*((unsigned int*)temp) = ID;
		temp+=4;

		*((unsigned int*)temp) = offset;
		temp+=4;

		*((unsigned int*)temp) = msg_length;
		temp+=4;

		*((unsigned char*)temp) = flag;
		temp+=1;


		snprintf((char*)temp, msg_length+1, "%s", raw_msg);
	}



	void *asBuf(){
		return rawPacket;
	}
	

	void print(){
		printf("Información de paquete...\n");
		printf("Proveniente desde %s:%u\n", long_addr_to_ip_str(addr.sin_addr.s_addr), addr.sin_port);
		printf("Dirección: %s\n", sdir);
		printf("Puerto: %u\n", puerto);
		printf("TTL: %u\n", ttl);
		printf("ID: %u\n", ID);
		printf("Offset: %u\n", offset);
		printf("Tamaño: %u\n", msg_length);
		printf("Flag: %u\n", flag);
		printf("Contenido:%s\n", raw_msg);

	}
	private:
		char sdir[16];
		void* rawPacket = malloc(HEADER_SIZE + MSG_SIZE);
};