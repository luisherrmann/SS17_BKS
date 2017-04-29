#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>

#include <stdio.h>	//TODO: printf durch syscall ersetzen und stdio.h Bibliothek entfernen

int copy(char *sourcename, char *targetname){
	int try_target = open(targetname,O_WRONLY);	
	if (try_target < 0){
		//fd von targetname < 0 => target file was not found ? andere Fehler? -> continue
		int fd_target = open(targetname,O_WRONLY | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWUSR);	
		int fd_source = open(sourcename,O_RDONLY);
		size_t bufSize = 128;		
		void *buf = malloc(bufSize);
		ssize_t writtenBytes = 0;		
		ssize_t readBytes = read(fd_source, buf, bufSize);
		while (readBytes > 0) {	
			writtenBytes = writtenBytes + write(fd_target,buf,readBytes);
			readBytes = read(fd_source, buf, bufSize);
		}
		printf("%zd Bytes geschrieben\n",writtenBytes);
		close(fd_source);
		close(fd_target);
	} 	
	close(try_target);
	
	return 0;
}
int main (){
	char *source = "test.txt";
	char *target = "target.txt";	
	copy(source,target);	
	return EXIT_SUCCESS;		
}

