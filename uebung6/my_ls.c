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
#include <stdbool.h>

int listFiles(char *dirname, bool showInvisible){
	DIR *dir = opendir(dirname);
	struct dirent *dirent = readdir(dir);
	while (dirent != NULL){
		struct stat st;
		off_t sizeT;
		char* filename = dirent->d_name;		
		if (stat(filename,&st)==0) sizeT=st.st_size;
		if (filename[0]!='.' || showInvisible)
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
		listFiles("./", false);
	}
	if (argc == 2){
		if (argv[1][0] == '-'){
		switch(argv[1][1]){
			case 'a': listFiles("./", true);break;
			case 'l': break;
		}}else {
			char tempPath[] = "./";
			strcat(tempPath,argv[1]);
			listFiles(tempPath,false);
		}
	}
	

	
		
	return EXIT_SUCCESS;
}
