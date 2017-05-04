#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>
#include "trashcan.h"

int fileExists(char *filename){
	int try_target = open(filename,O_WRONLY);
	int result =try_target;	
	close(try_target);	
	return (result<0);	
}

int copy(char *sourcename, char *targetname){
	if (fileExists(sourcename)!=0){
	const char message[]="Die zu löschende Datei wurde nicht gefunden. Der Vorgang wurde abgebrochen.\n";
		write(STDERR_FILENO,message,sizeof(message));
		exit(EXIT_FAILURE);
	}
	int try_target = open(targetname,O_WRONLY);	
	if (fileExists(targetname)){
		//fd von targetname < 0 => target file was not found ? andere Fehler? -> continue
		int fd_target = open(targetname,O_WRONLY | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWUSR);	
		int fd_source = open(sourcename,O_RDONLY);
		size_t bufSize = 128;		
		void *buf = malloc(bufSize);
		ssize_t writtenBytes = 0;		
		int tempWrittenBytes = 0;
		ssize_t readBytes = read(fd_source, buf, bufSize);
		while (readBytes > 0) {	
			while (tempWrittenBytes < readBytes){
				tempWrittenBytes = tempWrittenBytes + write(fd_target,*(&buf + tempWrittenBytes),readBytes-tempWrittenBytes);
			}
			writtenBytes = writtenBytes + tempWrittenBytes;	
			tempWrittenBytes=0;
			readBytes = read(fd_source, buf, bufSize);
		}
		free(buf);
		const char message[] ="Datei erfolgreich kopiert.\n";
		write(STDOUT_FILENO,message,sizeof(message));
		close(fd_source);
		close(fd_target);
		close(try_target);
	} else {
		const char message[] ="Die Datei existiert bereits im Zielverzeichnis. Der Vorgang wurde abgebrochen.\n";
		write(STDERR_FILENO,message,sizeof(message));
		close(try_target);		
		return -1;
	}
		
	return 0;
}
