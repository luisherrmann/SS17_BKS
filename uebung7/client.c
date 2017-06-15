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

#define MSG_LEN 500

struct shared_mem{
	int len;
	char buf[MSG_LEN];
	int received_message;
};

struct shared_mem *mem_ptr;
int writeToStdIn(){
	int fd_shared;	
	const char *filename = "/shared_space";
	fd_shared = shm_open(filename,O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	
	mem_ptr = mmap(NULL,sizeof(struct shared_mem), PROT_READ | PROT_WRITE, MAP_SHARED, fd_shared,0);

	char *buf_t = malloc(BUF_SIZE);	
	fgets(buf_t,BUF_SIZE,stdin);
	char curr;
	size_t i =0;
	while ((curr=fgetc(stdin))!=EOF && i<BUF_SIZE-1){
		buf_t[i]=(char)curr;
		i++;
	}
	buf_t[i]='\0';
	//strcpy(memPtr->buf,bufT);
	sprintf(mem_ptr->buf,"%s\n",buf_t);
	
	close(fd_shared);
	return EXIT_SUCCESS;
}

int main(int argc, char *argv[]){
	writeToStdIn();

	return EXIT_SUCCESS;
}

