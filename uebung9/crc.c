#define _DEFAULT_SOURCE //Aktiviert glibc-Erweiterungen

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <inttypes.h>
#include <string.h>

#define BUF_SIZE 100

//Expects: FILE struct pointer pointing to head of file
//Returns: Result of crc operation applied on file read with file descriptor
int crc_calc(FILE* source){
	int generator_polynomial = (1 << 16) + (1 << 15) + (1 << 2) + 1;
	int dividend = 0;
	//Since we insert two bytes before starting, we can always fetch two characters
	for(int i=0; i<2; i++){
		char byte = fgetc(source);
		dividend = (((int) dividend) << 8) + (int) byte;
	}
	char buffer_byte;
	
	//Start polynomial division
	int bit_count = 8;
	int eof_reached = 0;
	do{
		if(bit_count == 8){
			buffer_byte = fgetc(source);
			if(buffer_byte == EOF){
				eof_reached = 1;
				fprintf(stdout, "EOF reached!");
			}
			bit_count = 0;
		}
		if(eof_reached == 0){
			int next_bit = (int)((buffer_byte & (1 << (7 - bit_count))) >> (7 - bit_count));
			dividend = (dividend << 1) + next_bit;
			//If the first bit of the dividend is 0, fit 0, else fit 1
			if((dividend >> 16) == 1){
			       	dividend = dividend ^ generator_polynomial;
			}
			else{
			       	dividend = dividend ^ 0;
			}
			bit_count++;
		}
	}while(eof_reached == 0);
	return dividend;
}

int main(int argc, char **argv){
	if(argc == 2){
		int fn_len = strlen(argv[1]);
		//Falls *.crc, Checksumme prüfen
		if(strcmp(&argv[1][fn_len-4],".crc") == 0){
			char *filename = malloc(fn_len - 3);
			strncpy(filename, argv[1], fn_len-4);
			filename[fn_len-3] = '\0';
			FILE *source = fopen(argv[1],"r+");
			if(source == NULL){
				fprintf(stderr, "An error has occured while trying to read!\n");
				if(errno == ENOENT){
					fprintf(stderr, "The file specified does not exist!\n");
				}
				return 0;
			}
			if(crc_calc(source) == 0){
				fprintf(stdout, "Checksum matches!");
				fseek(source, 0, SEEK_END);
				int filelen = ftell(source);
				ftruncate(fileno(source), (off_t) filelen-2);
				fclose(source);
				rename(argv[1], filename);
				free(filename);
				return 1;
			}
			else{
				fprintf(stderr, "Checksum does not match! The file has been changed!\n");
				free(filename);
				return 0;
			}
		}
		//Sonst: Checksumme anhängen
		else{
			FILE *source = fopen(argv[1],"a+");
			if(source == NULL){
				fprintf(stderr, "An error has occured while trying to read and write!\n");
				if(errno == ENOENT){
					fprintf(stderr, "The file specified does not exist!\n");
				}
				return 0;
			}
			//Append 16 0's to end of file and start crc at initial position.
			fseek(source, 0, SEEK_END);
			int filelen = ftell(source);
			fprintf(stdout, "Length: %d\n", filelen);
			fprintf(source, "%c%c", 0, 0);
			rewind(source);
			fseek(source, 0, SEEK_END);
			filelen = ftell(source);
			fprintf(stdout, "Length: %d\n", filelen);
			rewind(source);
			int crc = crc_calc(source);
			char code1 = (char) (crc >> 8);
			char code2 = (char) (crc & 0xff);
			ftruncate(fileno(source), (off_t) filelen-2);
			fprintf(source, "%c%c", code1, code2);
			fprintf(stdout, "The checksum calculated is: %x\n", crc);
			fclose(source);
			int fn_len = strlen(argv[1]);
			char *filename_crc = malloc(fn_len + 5);
			memcpy(filename_crc, argv[1], fn_len);
			memcpy(&filename_crc[fn_len], ".crc", 5);
			rename(argv[1], filename_crc);
		}
	}
	else{
		fprintf(stderr, "Exactly one argument expected!\n");
		return 0;
	}
	return 1;
}
