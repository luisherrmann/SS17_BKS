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
#include <time.h>
#include <stdbool.h>

extern int64_t strToInt(const char*, char**, uint8_t);
extern void sort(uint64_t len, int64_t a[len]);

char* end = NULL;
struct linkedFileInfoList *headP = NULL;
uint64_t fileCounter = 0;
int64_t *curArray;

struct linkedFileInfoList{
	const char *filename;
	struct stat st;
	int64_t sortValue;
	long int dirPos;
	struct linkedFileInfoList *next;
};
int64_t getSortValue(const char *filename){
	int64_t tempIlast= strToInt(filename,&end,(uint64_t)36);
	int64_t curI=tempIlast;
	while (curI>=tempIlast){
		tempIlast=curI;
		curI*=36;
	}
	return tempIlast;
}
struct linkedFileInfoList *newLinkedFileInfoList(struct dirent *dirent, DIR *dir){
	struct linkedFileInfoList *new = malloc(sizeof(struct linkedFileInfoList));
	struct stat *stS = malloc(sizeof(struct stat));
	stat(dirent->d_name,stS);
	
	new->st = *stS;
	new->filename = dirent->d_name;

	new->dirPos = telldir(dir);
	new->sortValue = getSortValue(new->filename);
	
	new->next = NULL;	
	return new;
}

int initLinkedList(char *dirname, bool showInvisible){
	DIR *dir = opendir(dirname);
	struct dirent *dirent = readdir(dir);
	struct linkedFileInfoList *curNode = headP;
	struct linkedFileInfoList *lastNode = NULL;
	while (dirent != NULL){
		
		if (*dirent->d_name!='.' || showInvisible){
			curNode = newLinkedFileInfoList(dirent,dir);
			if (headP == NULL)  {headP=curNode; }
			else {lastNode->next = curNode;}
			lastNode = curNode;
			fileCounter++;
		}

		dirent = readdir(dir);
	}	
	closedir(dir);	
	return 0;
}
char *buff;
char *getStringDate(time_t time){
    buff=malloc(100); //TODO: dynamisch
    strftime (buff, 100, "%Y-%m-%d %H:%M:%S", localtime (&time));
    return buff;
}
int printNode(struct linkedFileInfoList *curNode){
	printf("%-20s %15li Byte \t Modified %s\n",curNode->filename,curNode->st.st_size,getStringDate(curNode->st.st_mtime));
	return EXIT_SUCCESS;
}
int printLinkedFileInfoList(){
	if (headP == NULL) {
		return -1;
	}
	struct linkedFileInfoList *curNode = headP;
	while (curNode != NULL){
		printNode(curNode);
		curNode=curNode->next;	
	}
	return EXIT_SUCCESS;

}

int createSortableArray(){
	curArray = (int64_t *)malloc(fileCounter*sizeof(int64_t));
	struct linkedFileInfoList *curNode = headP;
	for (uint64_t i = 0; i<fileCounter;i++){
		curArray[i]=curNode->sortValue;
		curNode=curNode->next;
	}
	return EXIT_SUCCESS;

}

void printSortedLS(){
	for (uint64_t i=0;i<fileCounter;i++){
		struct linkedFileInfoList *curNode = headP;
		struct linkedFileInfoList *lastNode = NULL;
		while (curNode != NULL){
			if (curArray[i]==curNode->sortValue){
				printNode(curNode);
				if (lastNode==NULL) 
					headP=curNode->next;
				else {
					lastNode->next = curNode->next;				
				}
			break;		
			}
			lastNode = curNode;
			curNode = curNode->next;
		}		
	}
}

int main(int argc, char *argv[]){
	if (argc > 3){
		const char message[] ="Zu viele Parameter für my_ls.\n";
		write(STDERR_FILENO,message,sizeof(message));
		return EXIT_FAILURE;
	}else if (argc==1){
		initLinkedList("./",false);
		printf("TEST\n");
		
		printLinkedFileInfoList();


	}
	if (argc == 2){
		if (argv[1][0] == '-'){
		switch(argv[1][1]){
			case 'a': initLinkedList("./", true); printLinkedFileInfoList(); break;
			case 'l': {
				initLinkedList("./",false);
 				printf("Verzeichnis ./ sortiert:\n");
				createSortableArray();	
				sort(fileCounter,curArray);
				printSortedLS(); 
				break;}
		}}else {
			char tempPath[] = "./";
			strcat(tempPath,argv[1]);
			initLinkedList("./",false);
			printLinkedFileInfoList();
		}
	}		
	return EXIT_SUCCESS;
}
