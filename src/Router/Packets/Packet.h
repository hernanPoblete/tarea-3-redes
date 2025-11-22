#include "../../config.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../utils.h"
#include <bits/stdc++.h>
using namespace std;



/**
 * LES JURO QUE NO ESTAMOS USANDO LLM. COMO NOS VUELVAN 
 * A BAJAR UN PUNTO Y TENGA QUE VOLVER A MANDAR UN MENSAJE A EQUIPO DOCENTE
 * Y TENGA QUE VOLVER A LA OFICINA DE LA PROFE IVANA A EXPLICAR TODO EL CODIGO
 * VOY A TERMINAR COLAPSANDO C:
 * 
 */

// TODO: ELIMINAR EL MENSAJE QUE ESCRIBI EN EL CODIGO DEL C3 PARA MANTENER SANIDAD DESPUES DE LAS NOTAS DEL C2

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

		void *current = rawPacket; // paquete en bytes turbocrudo
		for(int i = 0; i<4; i++){ // Draws address from the rawPacket
			direccion[i] = *((unsigned char*)current);
			current += 1;	// Recordatorio de que las alertas son solo sugerencias
		}

		current -= 4;

		snprintf(sdir, 16, "%hhu.%hhu.%hhu.%hhu", direccion[0], direccion[1], direccion[2], direccion[3]);

		for(int i = 0; i<status; i++){
			unsigned char* ptr = (unsigned char *)current;
			unsigned int num = *(ptr + i);

			//cout << num << ' ';
		}
		//cout << endl;

		current += 4;

		puerto = *((unsigned int*)(current)); //Extraer el puerto
		current+=4;

		unsigned char* ttlc = (unsigned char*)(current); // Recordatorio de que no tendría que estar haciendo este casteo si estuviera en C en vez de CPP
		ttl = *ttlc;

		//cout << puerto << " " << (int)ttl << endl;

		current += 1;

		
		ID = read_bytes(current, 4);
		current+=4;

		//cout << ID << endl;
	
		offset = read_bytes(current, 4);
		current+=4;

		//cout << offset << endl;
		
		msg_length = read_bytes(current, 4);
		current += 4;

		//cout << "msg_length: " << msg_length << endl;

		flag = *((unsigned char*) current);
		current+=1;
		
		unsigned char* ptr = (unsigned char*)current;
		for(int i = 0; i<msg_length; i++){
			raw_msg[i] = *(ptr+i);
			//cout << raw_msg[i];
		}
		raw_msg[msg_length] = 0;

		//cout << endl;

		//cout << "skibidi" << endl;
		
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
		snprintf(sdir, 16, "%hhu.%hhu.%hhu.%hhu", direccion[0], direccion[1], direccion[2], direccion[3]);

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


	bool operator<(const Packet& other){
		return offset < other.offset;
	}


	void *asBuf(){
		return rawPacket;
	}
	

	void print(){
		printf("Información de paquete...\n");
		printf("Proveniente desde %s:%u\n", long_addr_to_ip_str(addr.sin_addr.s_addr), htons(addr.sin_port));
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
		void* rawPacket = malloc(HEADER_SIZE + MSG_SIZE+1);
		unsigned int read_bytes(void *buffer, int amt){
			unsigned cnt = 0;
			unsigned int ans = 0;
			unsigned char* ptr = (unsigned char*)buffer;
			for(int i = 0; i<amt; i++){
				unsigned char c = *(ptr + i);

				ans += (unsigned int)c<<cnt;
				cnt += 8;
			}
			return ans;
		}
};