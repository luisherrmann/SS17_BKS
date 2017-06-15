#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>

// Buffer data structures
#define MSG_LEN 500

struct shared_mem{
	int len;
	char message[MSG_LEN];
	int received_message;
};

int main (int argc, char **argv)
{
	//Erzeuge und mappe geteilten Speicher 
	int fd_shm = shm_open("/shared_space", O_RDWR | O_CREAT, S_IWUSR | S_IRUSR | S_IWOTH | S_IROTH);
	if(fd_shm == -1){
		fprintf(stderr, "Error opening Shared Memory");
		return EXIT_FAILURE;
    	}

	if (ftruncate(fd_shm, sizeof(struct shared_mem)) == -1){
		fprintf(stderr, "Error extending Shared Memory");
		return EXIT_FAILURE;
    	}

    	struct shared_mem *shm;
	if(mmap(NULL, sizeof (struct shared_mem), PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0) == MAP_FAILED){
		fprintf(stderr, "Error mapping memory");
		return EXIT_FAILURE;
	}
	shm->received_message = 0;
	//Aktives Warten auf erhaltene Nachricht
	fprintf(stdout, "I am here");
	while(shm->received_message == 0){}
	char *message = malloc(strlen(shm->message));
	strcpy(message, shm->message);
	fprintf(stdout, "Client passed message: %s", message);
	close(fd_shm);
	shm_unlink("/shared_space");
	return EXIT_SUCCESS;
} 
