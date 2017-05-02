#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>
#include "trashcan.h"


int main (int argc, char *argv[]){
	if (argc !=3){
		const char message[] ="trashcan benötigt genau zwei Parameter.\n";
		write(STDERR_FILENO,message,sizeof(message));
		return EXIT_FAILURE;
	}
	struct stat dirInfoBuffer = {0};
	int checkDir = stat(".ti3_trashcan", &dirInfoBuffer);
	if (checkDir == -1) {
	    mkdir(".ti3_trashcan", 0755);
		const char message[] ="Trashcan nicht gefunden -> wurde erstellt.\n";
		write(STDERR_FILENO,message,sizeof(message));
	}

	
	char *source = "test.txt";
	char *target = "target.txt";	
	copy(source,target);	
	return EXIT_SUCCESS;		
}

