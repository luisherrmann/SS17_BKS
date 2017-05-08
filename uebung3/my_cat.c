#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include <string.h>

void printfile(FILE *source){
	int BUFFER_SIZE = 100;
	char *buffer = malloc(BUFFER_SIZE);
	if(source == NULL)
	{
		fprintf(stderr, "An error has occured while reading!\n");
		if(errno == ENOENT){
			fprintf(stderr, "The file specified does not exist!");
		}
	}
	int i = 0;
	char curr;
	while((curr = fgetc(source)) != EOF){
		buffer[i] = curr;
		//Reads until ESC is encountered.
		if(buffer[i] == 27) break;
		i++;
		if(((i+1) % BUFFER_SIZE) == 0){
			char *temp  = buffer;
			buffer = malloc(strlen(buffer) + BUFFER_SIZE);
			strcat(buffer, temp);
		}
	}
	fprintf(stdout,"%s\n",buffer);
	fclose(source);
}




int main(int argc, char* argv[]){
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
			}
		}
	}
	return EXIT_SUCCESS;
}
