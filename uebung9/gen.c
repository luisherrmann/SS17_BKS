
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
	fprintf(file1, "%c%c%c", (char)0x3f, (char)0xfd, (char)0x01);
	fclose(file1);

	FILE *file2 = fopen("gen2", "w+");
	fprintf(file2, "%c%c%c", (char)0xff, (char)0xff, (char)0x81);
	fclose(file2);
}
