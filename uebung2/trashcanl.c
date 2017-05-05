#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h> //Notwending fuer readdir()

#include <sys/types.h>
#include <sys/stat.h>

#include <string.h>

int copy(char *sourcename, char *targetname){
	int BUFFER_SIZE = 100;
	char buffer[100] = "Hello, this is a test.";
	int fd_target = open(targetname, O_WRONLY | O_APPEND | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH);
	//O_EXCL Flag garantiert, dass das File erzeugt werden muss. Falls bereits existent, wird Fehler erzeugt.
	if(fd_target < 0){
		if(errno == EEXIST){
			printf("The specified target file already exists!\n");
		}
		return EXIT_FAILURE;
	}
	int fd_source = open(sourcename, O_RDONLY);
	if(fd_source < 0){
		if(errno == ENOENT){
			printf("The specified source file does not exist!\n");
		}
		return EXIT_FAILURE;
	}
	ssize_t r;
	ssize_t w;
	do{
		r = read(fd_source,buffer,BUFFER_SIZE);
		if(r == -1){
			printf("An error occured while reading.\n");
			return EXIT_FAILURE;
		}
		//read() returns 0 when eof is reached.
		w=0;
		while(w < r){
			//Since written>=0, loop will only be entered if r >0)
			ssize_t tmp = write(fd_target,buffer,r - w);
			if(tmp == -1){
				printf("An error occured while writing.\n");
				return EXIT_FAILURE;
			}
			w += tmp;
		}
	}while(r != 0);
	close(fd_target);
	close(fd_source);
	return EXIT_SUCCESS;
}

void help(){
	printf("Your program call must have one of the following forms:\n\
		./trashcan <flag> <filename>\n\n\
		Flags:\n\
		-d\tDelete <filename> from active directory, copy to .trashcan\n\
		-r\tRecover <filename> from .trashcan to active directory, remove in .trashcan\n\
		-f\tDelete <filename> from .trashcan\n\n\
		./trashcan <flag>\n\n\
		Flags:\n\
		-l\tList all files in .trashcan\n\
		-h\tShow this help\n");
}

int main(int argc, char *argv[]){
	int fd_dir = mkdir(".trashcan", S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH);
	if(fd_dir == -1 && errno != EEXIST) return EXIT_FAILURE;
	if(argc == 2){
		if(strcmp(argv[1],"-l") == 0){
			void *dd = opendir(".trashcan");
			struct dirent *dir;
			while((dir = readdir(dd)) != NULL){
				printf("%s\n", dir->d_name);
			}
		}
		else if(strcmp(argv[1],"-h") == 0){
			help();
		}
		else{
			printf("Illegal input!\n");
			help();
			return EXIT_FAILURE;
		}
	}
	else if(argc == 3){
		char *pathdir = ".trashcan/";
		char *path = malloc(strlen(pathdir)+strlen(argv[2]));
		strcat(path,pathdir);
		strcat(path,argv[2]);
		if(strcmp(argv[1],"-d") == 0){
			copy(argv[2],path);
			if(remove(argv[2]) == -1){
				printf("Could not remove source file!\n");
				return EXIT_FAILURE;
			}
		}
		else if(strcmp(argv[1],"-r") == 0){
			copy(path,argv[2]);
			if(remove(path) == -1){
				printf("Could not remove copy in trashcan!\n");
				return EXIT_FAILURE;
			}
		}
		else if(strcmp(argv[1],"-f") == 0 && remove(path) == -1){
			printf("Could not remove copy in trashcan!\n");
			return EXIT_FAILURE;
		}
	}
	else{
		printf("Illegal input.\n");
		help();
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
