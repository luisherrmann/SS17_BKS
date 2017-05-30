#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>


int listFiles(char *dirname){
	DIR *dir = opendir(dirname);
	struct dirent *dirent = readdir(dir);
	while (dirent != NULL){
		struct stat st;
		off_t sizeT;
		char* filename = dirent->d_name;		
		if (stat(filename,&st)==0) sizeT=st.st_size;
		printf("%s\t\t%li Byte\n",filename,sizeT);
		dirent = readdir(dir);
	}	
	closedir(dir);	
	return 0;
}
int main(int argc, char *argv[]){

	if (argc > 3){
		const char message[] ="Zu viele Parameter für my_ls.\n";
		write(STDERR_FILENO,message,sizeof(message));
		return EXIT_FAILURE;
	}else if (argc==1){
		listFiles("./");
	}

	
		
	return EXIT_SUCCESS;
}
