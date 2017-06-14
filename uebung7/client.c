#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <inttypes.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

#define BUF_SIZE 500

struct sharedMem{
	int len;
	char buf[BUF_SIZE];
};
struct sharedMem *memPtr;
int writeToStdIn(){
	char *inputString = malloc(BUF_SIZE);
	int fdShared;
	fgets(inputString,BUF_SIZE,stdin);
	printf("%s\n",inputString);
	const char *filename = "/sharedSpace";
	fdShared = shm_open(filename,O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	if (ftruncate(fdShared, sizeof(struct sharedMem)) == -1)
		return EXIT_FAILURE;
	memPtr = mmap(NULL,sizeof(struct sharedMem), PROT_READ | PROT_WRITE, MAP_SHARED, fdShared,0);
	size_t tempBytesWritten = 0;
	void *buf = malloc(BUF_SIZE);
	do {
		tempBytesWritten += write(fdShared, buf, BUF_SIZE);	
	
	} while (tempBytesWritten<sizeof(inputString));
	close(fdShared);
	return EXIT_SUCCESS;
}
int main(int argc, char *argv[]){
	writeToStdIn();

	return EXIT_SUCCESS;
}

