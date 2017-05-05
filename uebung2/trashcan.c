#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <dirent.h>

#include "trashcan.h"

char *addTrashcanPath(char *filename){
	char *tempPath = ".ti3_trashcan/";
	char *path = malloc(sizeof(tempPath)+strlen(filename)); //use strnlen instead
	strncat(path,tempPath,strlen(tempPath));
	strncat(path,filename,strlen(filename));
	return path;
}
int deleteFile(char *filename){
	char *target = addTrashcanPath(filename);
	int resultCopy = copy(filename,target);
	int resultDel = 0;
	if (resultCopy==0) unlink(filename);
	free(target);
	return resultCopy+resultDel;
}
int recoverFile(char *filename){
	char *trashcanPath = addTrashcanPath(filename);
	int resultCopy = copy(trashcanPath,filename);
	int resultDel = (resultCopy==0)?unlink(trashcanPath):0;
	free(trashcanPath);
	return resultCopy+resultDel;
}
int fDelFile(char *filename){
	char *trashcanPath = addTrashcanPath(filename);
	int result = unlink(trashcanPath);
	free(trashcanPath);
	return result;	

}
int listTrashcanFiles(){
	DIR *trashcan = opendir(addTrashcanPath(""));
	struct dirent *dirent = readdir(trashcan);
	while (dirent != NULL){
		char* filename = dirent->d_name;
		strncat(filename,"\n",2);
		write(STDOUT_FILENO,filename,strlen(filename));
		dirent = readdir(trashcan);
	}	
	closedir(trashcan);	
	return 0;
}	
int main (int argc, char *argv[]){
	if (argc !=3 && strcasecmp(argv[1],"-l")!=0){
		const char message[] ="trashcan benötigt genau mehr Parameter.\n";
		write(STDERR_FILENO,message,sizeof(message));
		return EXIT_FAILURE;
	}else if (strcasecmp(argv[1],"-l")==0){
		listTrashcanFiles();
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
	}else if (strcasecmp(argv[1],"-r")==0){
		const char message[] = "Recover\n";
		write(STDOUT_FILENO,message,sizeof(message));
		recoverFile(argv[2]);
	}else if (strcasecmp(argv[1],"-f")==0){
		const char message[] = "Finally Delete\n";
		write(STDOUT_FILENO,message,sizeof(message));
		fDelFile(argv[2]);
	}
		
	return EXIT_SUCCESS;		
}


