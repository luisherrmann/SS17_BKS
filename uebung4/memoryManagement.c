#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define MEM_SIZE 4096
#define TRUE 0
#define FALSE 1


struct LinkedMemoryList *headP;

char *memory[MEM_SIZE];

struct LinkedMemoryList{
	uint8_t isDeRef; // Speichert ob Knoten dereferenziert ist	
	size_t memSize;	//Speicher des gesamten Knoten (angeforderte memSizeContent + Speicher für den Knotenkopf)
	struct LinkedMemoryList *next;	//Nachfolger
};


struct LinkedMemoryList *newLinkedListNode(void *memStartPointer, size_t memSizeContent, struct LinkedMemoryList *nextNode){
	struct LinkedMemoryList *new = memStartPointer;
	new->memSize=memSizeContent+sizeof(struct LinkedMemoryList);
	new->isDeRef=(uint8_t)FALSE;
	new->next = nextNode;
	return new;
}



void* memory_allocate(size_t byte_count){ 

	struct LinkedMemoryList *curNode = headP;
	size_t curSpaceByteCount = curNode->memSize;
	struct LinkedMemoryList *derefNode = NULL;	//zeigt auf den ersten, dereferenzierten Knoten
	struct LinkedMemoryList *newMemNode = NULL;
	while (curNode->next != NULL){
		curNode = curNode->next;
		curSpaceByteCount +=curNode->memSize;		
		if (curNode->isDeRef == TRUE){	//Aktueller Knoten ist dereferenziert		
			if (derefNode == NULL){	//erster zusammenhängender dereferenzierter Knoten
				derefNode = curNode;	
			} else{	//Zusammenlegen zweier dereferenzierter Knoten
				derefNode->memSize += curNode->memSize;
				derefNode->next = curNode->next;
			}

			if (derefNode->memSize >= byte_count+sizeof(struct LinkedMemoryList)){
			//Neuer Speicher passt in das aktuelle, dereferenzierte Element
				size_t tempMemDiff = derefNode->memSize - byte_count - sizeof(struct LinkedMemoryList);
				derefNode->isDeRef =(uint8_t)FALSE;
				if (tempMemDiff >= sizeof(struct LinkedMemoryList)){
				//Falls genug Speicher zw. neuem & + nächsten Knoten übrig ist -> neuer dereferenzierter Knoten
					derefNode->memSize = byte_count+sizeof(struct LinkedMemoryList);
					newMemNode = newLinkedListNode(((void *)((char *)derefNode+derefNode->memSize)),tempMemDiff-sizeof(struct LinkedMemoryList),derefNode->next);
					newMemNode->isDeRef = (uint8_t)TRUE;
					derefNode->next = newMemNode;		
				}
				//Nicht genügend Speicher für neuen, leeren Knoten Knoten -> Speicher des alten Knoten bleibt bestehen
				//-> Speichergröße ist memSize >= byte_count und memSize < byte_count + sizeof(struct LinkedMemoryList)
				
				return (void *)(derefNode+sizeof(struct LinkedMemoryList));			
			}	
		} else derefNode = NULL;
	}
	if (curSpaceByteCount + sizeof(struct LinkedMemoryList) + byte_count <= MEM_SIZE){
		newMemNode = newLinkedListNode(((void *)(&curNode+curNode->memSize)),byte_count,NULL);
		curNode->next = newMemNode;
		return (void *)(newMemNode+sizeof(struct LinkedMemoryList));
	} else return NULL;
}
void memory_free(void *pointer){
		void *tempPV = (void *)(((struct LinkedMemoryList *)pointer)-sizeof(struct LinkedMemoryList));
		struct LinkedMemoryList *node = (struct LinkedMemoryList *)(tempPV);	
		node->isDeRef=(uint8_t)TRUE;

}	
void memory_print(){
	printf("Aktueller Speicher:\n");
	struct LinkedMemoryList *curNode = headP;
	while (curNode != NULL){
		printf("(Node:DeRef=%" PRIu8 ", Size=%li, Content-Size=%li, Content-Pointer=%p) ->\n",curNode->isDeRef,curNode->memSize, curNode->memSize-sizeof(struct LinkedMemoryList),(void *)(curNode+sizeof(struct LinkedMemoryList)));
		curNode=curNode->next;
	}
}

void memory_init(){
	headP= newLinkedListNode((void *)memory,0,NULL);

}

