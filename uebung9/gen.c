
#define _DEFAULT_SOURCE //Aktiviert glibc-Erweiterungen

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <inttypes.h>
#include <string.h>

int main(int agc, char **argv){
	int generator = (1 << 16) + (1 << 15) + (1 << 2) + 1;
	FILE *file1 = fopen("gen1", "w+");
	fprintf(file1, "%s%c", "asd", 0x0a);//0x3f, 0xfd, 0x01);
	fclose(file1);

	FILE *file2 = fopen("gen2", "w+");
	fprintf(file2, "%c%c%c", 0xff, 0xff, 0x81);
	fclose(file2);
}
