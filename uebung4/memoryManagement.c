#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <inttypes.h>

#define MEM_SIZE 4096;
struct LinkedMemoryList *headP;

char *memory[MEM_SIZE];

struct LinkedMemoryList{
	char isDeRef;	
	size_t memSize;
	struct LinkedMemoryList *next;
	struct LinkedMemoryList *head;
	void *memoryStartPointer;
};


struct LinkedMemoryList *newLinkedListNode(void *memStartPointer, size_t memSizeContent, struct LinkedMemoryList *headP){
	struct LinkedMemoryList *new = memStartPointer;
	new.memSize=memSizeContent+sizeof(struct LinkedMemoryList);
	new.isDeRef='0';
	new->memoryStartPointer=memStartPointer;
	new->head = headP;
	return new;
}


void memory_init(){
	headP= newLinkedListNode((void *)memory,0,NULL);

}
void* getNextFreeMem(size_t byte_count){ 
//Alternative: Dereferenzieren von Knoten nur durch ändern der boolschen Variable + Zusammenfügen von zusammenhängenden, dereferenzierten Knoten
	size_t curSpaceByteCount = 0;
	void *curSpaceStartPointer = memory;
	for (char *ptr=memory; ptr != memory + MEM_SIZE;++ptr){
		if (*ptr=='0' OR *ptr=='1'){
			size_t cur_size = (size_t)ptr+1;	
			if (*ptr=='0') {
				ptr +=cur_size;
				curSpaceByteCount = 0;
				curSpaceStartPointer = ptr;
			} else 
				curSpaceByteCount += cur_size;
		} else ++curSpaceByteCount;
		if (curSpaceByteCount >= byte_count){
			for (char *pmr=curSpaceStartPointer+curSpaceByteCount; pmr != Memory+MEM_SIZE;++pmr){
				**pmr=NULL;//??				
			}		
		}
	}
}
void* memory_allocate(size_t byte_count){
	return NULL;
}

void memory_free(void *pointer){
}
void memory_print(){
}


int main(){
	//TODO: main-Funktion von der MemoryManagement trennen.
	
	memory_init();
	return EXIT_SUCCESS;
}
