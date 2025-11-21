#include "Defragmentator.h"




DefragTree* makeDefragTree(){
	DefragTree *dt = (DefragTree*) malloc(sizeof(DefragTree));
	dt->current = NULL;
	dt->left = NULL;
	dt->right = NULL;

	return dt;
}


void addPacket(DefragTree *t, Packet* toAdd){
	if(t->current == NULL){
		t->current = toAdd;
		t->left = makeDefragTree();
		t->right = makeDefragTree();
	}else if(toAdd->offset < t->current->offset){
		addPacket(t->left, toAdd);
	}else{
		addPacket(t->right, toAdd);
	}
};


void printTree(DefragTree *t){

	if(t==NULL) return;

	DefragTree *left = t->left;
	DefragTree *right = t->right;
	Packet *current = t->current;

	if(current == NULL) return;
	
	printTree(left);
	current->print();
	printTree(right);
}

