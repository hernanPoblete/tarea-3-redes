#include "Packet.h"
#include <stdlib.h>
#include <bits/stdc++.h>
#include <string>
using namespace std;


typedef struct defragtree{
	Packet *current;
	struct defragtree *left;
	struct defragtree *right;
} DefragTree;

DefragTree* makeDefragTree();
void destroyDefragTree(DefragTree *t);
void addPacket(DefragTree *t, Packet* toAdd);
void printTree(DefragTree *t);


class Defragmentator{


	public:
		void addFrag(Packet pack){
			/*
			if(treeMap.find(pack.ID) == treeMap.end()){
				treeMap[pack.ID] = makeDefragTree();
			}

			addPacket(treeMap[pack.ID], &pack);
			*/
			packetMap[pack.ID].push_back(pack);
		}


		void print(unsigned int id){
			/*
			if(treeMap.find(id) != treeMap.end()){
				DefragTree* head = treeMap[id];
				printTree(head);
			}*/
			char* msg = join_packets(id);
			printf("%s\n", msg);
			free(msg);
		}

		char* join_packets(unsigned int id){
			sort(packetMap[id].begin(), packetMap[id].end());
			int last = packetMap[id].size() - 1;
			if(packetMap[id][last].flag != 0){
				return NULL;
			}
			
			string ans = "";
			// check message integrity
			int curr = 0;
			for(Packet p: packetMap[id]){
				if(p.offset != curr){
					return NULL;
				}
				curr += p.msg_length;
			}

			// if check is successful, build the message
			for(Packet p: packetMap[id]){
				ans += (string)p.raw_msg; // completamente innecesario pero era mas rapido de codear XD
			}

			char* message = (char*)malloc(curr + 1);
			for(int i = 0; i < curr; i++){
				message[i] = ans[i];
			}
			message[curr] = '\0';
			return message;
		}


	private:
		//map<unsigned int, DefragTree*> treeMap;
		map<unsigned int, vector<Packet>> packetMap;

};