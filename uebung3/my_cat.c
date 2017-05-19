#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <inttypes.h>

#define BUFFER_SIZE 128

int printfile(FILE *source){

	char *buffer = malloc(BUFFER_SIZE);
	if(source == NULL)
	{
		fprintf(stderr, "An error has occured while reading!\n");
		if(errno == ENOENT){
			fprintf(stderr, "The file specified does not exist!\n");
		}
		return 0;
	}
	char *content = malloc(BUFFER_SIZE);
	while(fgets(buffer,BUFFER_SIZE,source) != NULL){
		char* temp = malloc(strlen(content) + strlen(buffer) + 1);
		strcpy(temp,content);
		strcat(temp,buffer);
		content = temp;
	}
	free(buffer);
	fprintf(stdout,"%s\n",content);
	free(content);
	fclose(source);
	return 1;
}

int main(int argc, char* argv[]){
	int status;
	if(argc == 1){
		fprintf(stdout, "Reading from stdin\n\n");
		int ret = printfile(stdin);
		if(ret == 0) return 0;
	}
	else if(argc == 2){
		FILE *source = fopen(argv[1],"r");
		int ret = printfile(source);
		if(ret == 0) return 0;
	}
	else{
		int i;
		for(i = 1; i < argc; i++){
			int pid = fork();
			if(pid == 0){//Falls Fork Kindprozess ist, fuehre Aufgabe durch.
				FILE *source = fopen(argv[i],"r");
				int ret = printfile(source);
				if(ret == 0) return 0;
				break;
			}
			else {//Sonst warte auf Kindprozesse
				wait(&status);
			}
		}
	}
	return 1;
}
