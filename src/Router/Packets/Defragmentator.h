#include "Packet.h"
#include <stdlib.h>
#include <bits/stdc++.h>
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
			if(treeMap.find(pack.ID) == treeMap.end()){
				treeMap[pack.ID] = makeDefragTree();
			}

			addPacket(treeMap[pack.ID], &pack);
		}


		void print(unsigned int id){
			if(treeMap.find(id) != treeMap.end()){
				DefragTree* head = treeMap[id];
				printTree(head);
			}
		}




	private:
		map<unsigned int, DefragTree*> treeMap;

};