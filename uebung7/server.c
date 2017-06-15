//Compile with gcc -std=c11 -Wall -Wextra -pedantic -o client client.c -lrt -pthread
#define _DEFAULT_SOURCE //Aktiviert glibc-Erweiterungen
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>
#include <sys/mman.h>

#define MSG_LEN 500

//Buffer Datenstruktur
struct shared_mem{
	int len;
	char message[MSG_LEN];
	//int received_message;
};

int main ()
{
	//Erzeuge POSIX-Semaphore um aktives Warten zu vermeiden
	sem_t *fd_sem;
	if((fd_sem = sem_open("/shm_sem", O_CREAT, S_IWUSR | S_IRUSR | S_IWOTH | S_IROTH, 0)) == SEM_FAILED){
		fprintf(stderr, "Could not create semaphore: ERRNO %i\n", errno);
		return EXIT_FAILURE;
	}
	//Erzeuge und mappe geteilten Speicher 
	int fd_shm = shm_open("/shared_space", O_RDWR | O_CREAT, S_IWUSR | S_IRUSR | S_IWOTH | S_IROTH);
	if(fd_shm == -1){
		fprintf(stderr, "Error opening Shared Memory: ERRNO %i\n", errno);
		return EXIT_FAILURE;
    	}
	//Vergrößere den geteilten Speicher auf die Größe eines struct shared_mem
	if (ftruncate(fd_shm, sizeof(struct shared_mem)) == -1){
		fprintf(stderr, "Error extending Shared Memory: ERRNO %i\n", errno);
		return EXIT_FAILURE;
    	}

    	struct shared_mem *shm;
	if((shm = mmap(NULL, sizeof (struct shared_mem), PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0)) == MAP_FAILED){
		fprintf(stderr, "Error mapping memory: ERRNO %i\n", errno);
		return EXIT_FAILURE;
	}
	//shm->received_message = 0;
	//Aktives Warten auf erhaltene Nachricht
	//while(shm->received_message == 0){}
	sem_wait(fd_sem);
	char *message = malloc(strlen(shm->message));
	strcpy(message, shm->message);
	fprintf(stdout, "\nClient passed message: %s\n", message);
	close(fd_shm);
	sem_close(fd_sem);
	shm_unlink("/shared_space");
	return EXIT_SUCCESS;
} 
