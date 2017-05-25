#include <stdlib.h>
#include <stdio.h>
#include "memmanage.h"

int main(){
	memory_init();	

	void *temp = memory_allocate(512);
	memory_print();

	void *temp3 = memory_allocate(40);
	memory_print();

	memory_free(temp);
	memory_print();

	void *temp4 =memory_allocate(128);
	memory_print();	

	memory_free(temp4);
	memory_print();	

	void *temp5 = memory_allocate(350);
	memory_print();

	void *temp6 = memory_allocate(114);
	memory_print();
	printf("TestNodes Content-Pointers: %p, %p, %p\n",temp3,temp5,temp6);
	return EXIT_SUCCESS;
}
