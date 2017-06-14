//Compile with gcc -std=c11 -Wall -Wextra -pedantic -o client client.c -lrt
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <inttypes.h>
#include <unistd.h>
#include <strings.h>

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/mman.h>

#define BUF_SIZE 500

struct sharedMem{
	int len;
	const char *buf[BUF_SIZE];
};
struct sharedMem *memPtr;
int writeToStdIn(){
	int fdShared;	
	const char *filename = "/sharedSpace";
	fdShared = shm_open(filename,O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	if (ftruncate(fdShared, sizeof(struct sharedMem)) == -1)
		return EXIT_FAILURE;
	memPtr = mmap(NULL,sizeof(struct sharedMem), PROT_READ | PROT_WRITE, MAP_SHARED, fdShared,0);
	//printf("%i",memPtr->len);	
	//memPtr->len = 5;
	//memPtr->buf= malloc(BUF_SIZE);
	const char *bufT[BUF_SIZE];	
	fgets(bufT,BUF_SIZE,stdin);
	
	strcpy(memPtr->buf,bufT);
//	printf("%s\n",memPtr->buf);
	
	close(fdShared);
	return EXIT_SUCCESS;
}
int main(int argc, char *argv[]){
	writeToStdIn();

	return EXIT_SUCCESS;
}

