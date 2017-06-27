#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int wordcount(FILE *source){
	while((curr = fgetc(source)) != EOF){
}




int main(int argc, char* argv[]){
	FILE *source = fopen(argv[i],"r+");
	if(source == NULL)
	{
		fprintf(stderr, "An error has occured while trying to read!\n");
		if(errno == ENOENT){
			fprintf(stderr, "The file specified does not exist!\n");
		}
		return 0;
	}
	uint16_t generator_polynomial = 1*2**5 + 1*2**1;
	uint8_t byte1 = fgetc(source);
	uint8_t byte2 = fgetc(source);
	uint16_t dividend = (((uint16_t)byte1) << 8) | (uint16_t)byte2;
	uint16_t buffer_byte;
	//Erster Charakter ist eine 1.
	if(dividend >= 2** )

	while((buffer_byte = fgetc(source)) != EOF){
		uint16_t quotient = dividend ^ generator_polynomial;
		quotient == 
	}

	return 1;
}
