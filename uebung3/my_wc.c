#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include <string.h>

void wordcount(FILE *source){
	int chars = 0;
	int words = 0;
	int neww = 0;
	int lines = 0;
	if(source == NULL)
	{
		fprintf(stderr, "An error has occured while reading!\n");
		if(errno == ENOENT){
			fprintf(stderr, "The file specified does not exist!");
		}
	}
	char curr;
	while((curr = fgetc(source)) != EOF){
		//Reads until ESC is encountered.
		if(curr == 27) break;
		else if(curr == ' ') neww = 0;
		else if(curr == '\n'){
			lines++;
			neww = 0;
		}
		else{
			if(neww == 0){
				words++;
				neww = 1;
			}
		}
		chars++;
	}
	fprintf(stdout,"The file contains %d bytes, %d words and %d lines.\n",chars,words,lines);
	fclose(source);
}




int main(int argc, char* argv[]){
	if(argc == 1){
		fprintf(stdout, "Reading from stdin\n\n");
		wordcount(stdin);
	}
	if(argc == 2){
		FILE *source = fopen(argv[1],"r");
		wordcount(source);
	}
	else{
		int i;
		for(i = 1; i < argc; i++){
			int pid = fork();
			if(pid == 0){
				FILE *source = fopen(argv[i],"r");
				wordcount(source);
				break;
			}
		}
	}
	return EXIT_SUCCESS;
}
