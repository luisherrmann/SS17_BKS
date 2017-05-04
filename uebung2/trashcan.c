#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>

#include "trashcan.h"

int deleteFile(char *filename){
	char *targetTemp = ".ti3_trashcan/";
	char *target = malloc(sizeof(targetTemp)+strlen(filename));
	strncat(target,targetTemp,strlen(targetTemp));	// use strnlen instead
	strncat(target,filename,strlen(filename)); 
	int resultCopy = copy(filename,target);
	int resultDel = 0;
	if (resultCopy==0) unlink(filename);
	return resultCopy+resultDel;
}
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
	if (strcasecmp(argv[1],"-d")==0){
		const char message[] ="Delete\n";
		write(STDOUT_FILENO,message,sizeof(message));
		deleteFile(argv[2]);
	}else if (strcasecmp(argv[1],"-l")==0){
	}else if (strcasecmp(argv[1],"-r")==0){
	}else if (strcasecmp(argv[1],"-f")==0){
	}
		
	return EXIT_SUCCESS;		
}


