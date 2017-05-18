#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <inttypes.h>

#define BUFFER_SIZE 128

void printfile(FILE *source){

	char *buffer = malloc(BUFFER_SIZE);
	if(source == NULL)
	{
		fprintf(stderr, "An error has occured while reading!\n");
		if(errno == ENOENT){
			fprintf(stderr, "The file specified does not exist!");
		}
	}
	fseek(source, 0L, SEEK_END);	//search for EOF
	long int size = ftell(source);	//position of EOF is the size of the file in Bytes
	rewind(source);			//File wird wieder auf den Anfang gesetzt
	char *temp = malloc(size);
	while((buffer = fgets(buffer,BUFFER_SIZE,source))!=NULL){ //use fgets(bufferSize) instead
		
		strncat(temp,buffer,strlen(buffer));
		
	}
	free(buffer);
	fprintf(stdout,"%s\n",temp);
	free(temp);
	fclose(source);
}




int main(int argc, char* argv[]){
	int status;
	if(argc == 1){
		fprintf(stdout, "Reading from stdin\n\n");
		printfile(stdin);
	}
	if(argc == 2){
		FILE *source = fopen(argv[1],"r");
		printfile(source);
	}
	else{
		int i;
		for(i = 1; i < argc; i++){
			int pid = fork();
			if(pid == 0){
				FILE *source = fopen(argv[i],"r");
				printfile(source);
				break;
			}else {
				wait(&status);
			}
		}
	}
	return EXIT_SUCCESS;
}
