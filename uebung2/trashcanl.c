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
			char *msg = "The specified target file already exists!\n";
			write(STDERR_FILENO, msg, strlen(msg));
		}
		return EXIT_FAILURE;
	}
	int fd_source = open(sourcename, O_RDONLY);
	if(fd_source < 0){
		if(errno == ENOENT){
			char *msg = "The specified source file does not exist!\n";
			write(STDERR_FILENO, msg, strlen(msg));
		}
		return EXIT_FAILURE;
	}
	ssize_t r;
	ssize_t w;
	do{
		r = read(fd_source,buffer,BUFFER_SIZE);
		if(r == -1){
			char *msg = "An error occured while reading.\n";
			write(STDERR_FILENO, msg, strlen(msg));
			return EXIT_FAILURE;
		}
		//read() returns 0 when eof is reached.
		w=0;
		while(w < r){
			//Since written>=0, loop will only be entered if r >0)
			ssize_t tmp = write(fd_target,buffer,r - w);
			if(tmp == -1){
				char *msg = "An error occured while writing.\n";
				write(STDERR_FILENO, msg, strlen(msg));
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
	char * msg = "Your program call must have one of the following forms:\n\
		      ./trashcan <flag> <filename>\n\n\
		      Flags:\n\
		      -d\tDelete <filename> from active directory, copy to .ti3_trashcan\n\
		      -r\tRecover <filename> from .ti3_trashcan to active directory, remove in .ti3_trashcan\n\
		      -f\tDelete <filename> from .ti3_trashcan\n\n\
		      ./trashcan <flag>\n\n\
		      Flags:\n\
		      -l\tList all files in .ti3_trashcan\n\
		      -h\tShow this help\n";
	write(STDOUT_FILENO, msg, strlen(msg));
}

int illegal_input(){
	char *msg = "Illegal input.\n";
	write(STDERR_FILENO, msg, strlen(msg));
	help();
	return EXIT_FAILURE;
}

int main(int argc, char *argv[]){
	int fd_dir = mkdir(".ti3_trashcan", S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH);
	if(fd_dir == -1 && errno != EEXIST) return EXIT_FAILURE;
	if(argc == 2){
		if(strcmp(argv[1],"-l") == 0){
			void *dd = opendir(".ti3_trashcan");
			struct dirent *dir;
			while((dir = readdir(dd)) != NULL){
				char *msg = malloc(strlen(dir->d_name) + 1);
				strcat(msg, dir->d_name);
				strcat(msg, "\n");
				write(STDERR_FILENO, msg, strlen(msg));
			}
		}
		else if(strcmp(argv[1],"-h") == 0){
			help();
		}
		else{
			return illegal_input();
		}
	}
	else if(argc == 3){
		char *pathdir = ".ti3_trashcan/";
		char *path = malloc(strlen(pathdir)+strlen(argv[2]));
		strcat(path,pathdir);
		strcat(path,argv[2]);
		if(strcmp(argv[1],"-d") == 0){
			copy(argv[2],path);
			if(remove(argv[2]) == -1){
				char *msg = "Could not remove source file!\n";
				write(STDERR_FILENO, msg, strlen(msg));
				return EXIT_FAILURE;
			}
		}
		else if(strcmp(argv[1],"-r") == 0){
			copy(path,argv[2]);
			if(remove(path) == -1){
				char *msg = "Could not remove copy in trashcan!\n";
				write(STDERR_FILENO, msg, strlen(msg));
				return EXIT_FAILURE;
			}
		}
		else if(strcmp(argv[1],"-f") == 0 && remove(path) == -1){
			char *msg = "Could not remove copy in trashcan!\n";
			write(STDERR_FILENO, msg, strlen(msg));
			return EXIT_FAILURE;
		}
		else{
			return illegal_input();
		}
	}
	else{
		return illegal_input();
	}
	return EXIT_SUCCESS;
}
