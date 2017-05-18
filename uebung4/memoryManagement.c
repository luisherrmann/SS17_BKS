#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <inttypes.h>

#define MEM_SIZE 4096
#define TRUE 0
#define FALSE 1


struct LinkedMemoryList *headP;

char *memory[MEM_SIZE];

struct LinkedMemoryList{
	uint8_t isDeRef;	
	size_t memSize;
	struct LinkedMemoryList *next;
	void *memoryStartPointer;
};


struct LinkedMemoryList *newLinkedListNode(void *memStartPointer, size_t memSizeContent, struct LinkedMemoryList *nextNode){
	struct LinkedMemoryList *new = memStartPointer;
	new->memSize=memSizeContent+sizeof(struct LinkedMemoryList);
	new->isDeRef=FALSE;
	new->memoryStartPointer=memStartPointer;
	new->next = nextNode;
	return new;
}



void* memory_allocate(size_t byte_count){ 
//Alternative: Dereferenzieren von Knoten nur durch ändern der boolschen Variable + Zusammenfügen von zusammenhängenden, dereferenzierten Knoten

	struct LinkedMemoryList *curNode = headP;
	size_t curSpaceByteCount = curNode->memSize;
	struct LinkedMemoryList *derefNode = NULL;
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
			if (curNode->memSize >= byte_count+sizeof(struct LinkedMemoryList)){ 
			//Neuer Speicher passt in das aktuelle, dereferenzierte Element
				size_t tempMemDiff = curNode->memSize - byte_count - sizeof(struct LinkedMemoryList);
				if (tempMemDiff >= sizeof(struct LinkedMemoryList)){
				//Falls genug Speicher in der Lücke übrig ist,
					curNode->memSize = byte_count+sizeof(struct LinkedMemoryList);
					newMemNode = newLinkedListNode(((void *)((char *)curNode+curNode->memSize)),tempMemDiff-sizeof(struct LinkedMemoryList),curNode->next);
					curNode->next = newMemNode;		
				}
				return (void *)(curNode+sizeof(struct LinkedMemoryList));			
			}	
		} else derefNode = NULL;
	}
	if (curSpaceByteCount + sizeof(struct LinkedMemoryList) + byte_count <= MEM_SIZE){
		newMemNode = newLinkedListNode(((void *)(&curNode+curNode->memSize)),byte_count,NULL);
		curNode->next = newMemNode;
		return (void *)newMemNode;
	} else return NULL;
}
void memory_free(void *pointer){
	int size = (int)sizeof(struct LinkedMemoryList);
	struct LinkedMemoryList *node = (void *)((char *)pointer-size);
	node->isDeRef=TRUE;
}
void memory_print(){
}

void memory_init(){
	headP= newLinkedListNode((void *)memory,0,NULL);
	void *temp = memory_allocate(512);
	void *temp3 = memory_allocate(40);
	memory_free(temp);
	void *temp4 = memory_allocate(128);
	memory_free(temp3);
	memory_free(temp4);
}
int main(){
	//TODO: main-Funktion von der MemoryManagement trennen.
	
	memory_init();
	return EXIT_SUCCESS;
}
