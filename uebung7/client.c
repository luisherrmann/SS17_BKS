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
#include <semaphore.h>
#include <sys/mman.h>

#define MSG_LEN 500

struct shared_mem{
	int len;
	char buf[MSG_LEN];
	//int received_message;
};

struct shared_mem *shm;
int write_to_stdin(){	
	const char *filename = "/shared_space";
	sem_t *fd_sem;
	if((fd_sem = sem_open("/shm_sem", 0, 0, 0)) == SEM_FAILED){
		fprintf(stderr, "Could not open semaphore: ERRNO %i\n", errno);
		return EXIT_FAILURE;
	}
	int fd_shm = shm_open(filename, O_RDWR, S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH);
	if(fd_shm == -1){
		fprintf(stderr, "Error opening Shared Memory");
		return EXIT_FAILURE;
	}
	if((shm = mmap(NULL,sizeof(struct shared_mem), PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm,0)) == MAP_FAILED){
		fprintf(stderr, "Error mapping memory: ERRNO %i\n", errno);
		return EXIT_FAILURE;
	}

	char *buf_t = malloc(MSG_LEN);	
	char curr;
	size_t i =0;
	while ((curr=fgetc(stdin))!=EOF && i<MSG_LEN-1){
		buf_t[i]=(char)curr;
		i++;
	}
	buf_t[i]='\0';
	strcpy(shm->buf,buf_t);
	//sprintf(shm->buf,"%s\n",buf_t);
	//shm->received_message = 1;	
	sem_post(fd_sem);
	close(fd_shm);
	sem_close(fd_sem);
	return EXIT_SUCCESS;
}

int main(int argc, char *argv[]){
	write_to_stdin();
	return EXIT_SUCCESS;
}

