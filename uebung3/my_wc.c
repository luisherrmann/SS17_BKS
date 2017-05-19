#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int wordcount(FILE *source){
	int chars = 0;
	int words = 0;
	int neww = 0;	//1 fuer begonnenes neues Wort
	int lines = 0;
	if(source == NULL)
	{
		fprintf(stderr, "An error has occured while reading!\n");
		if(errno == ENOENT){
			fprintf(stderr, "The file specified does not exist!\n");
		}
		return 0;
	}
	char curr;
	while((curr = fgetc(source)) != EOF){
		if(curr == ' ') neww = 0;
		else if(curr == '\n'){
			lines++;
			neww = 0;
		}
		else{
			if(neww == 0){//Neues Wort begonnen
				words++;
				neww = 1;
			}
		}
		chars++;
	}
	fprintf(stdout,"The file contains %d bytes, %d words and %d lines.\n",chars,words,lines);
	fclose(source);
	return 1;
}




int main(int argc, char* argv[]){
	int status;
	if(argc == 1){
		fprintf(stdout, "Reading from stdin\n\n");
		int ret = wordcount(stdin);
		if(ret == 0) return 0;
	}
	else if(argc == 2){
		FILE *source = fopen(argv[1],"r");
		int ret = wordcount(source);
		if(ret == 0) return 0;
	}
	else{
		int i;
		for(i = 1; i < argc; i++){
			int pid = fork();
			if(pid == 0){//Falls aktueller Fork Kindprozess ist, fuere Aufgabe aus
				FILE *source = fopen(argv[i],"r");
				int ret = wordcount(source);
				if(ret == 0) return 0;
				break;
			}
			else{
				wait(&status);//Sonst warte auf Kindprozesse
			}
		}
	}
	return 1;
}
