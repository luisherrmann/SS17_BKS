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
#include <errno.h>

struct linkedFileInfoList *headP = NULL;
uint64_t fileCounter = 0;
struct linkedFileInfoList *curArray;
char curDirName[40] = "./";
int dirOrFileCounter=1;


struct linkedFileInfoList{
	char filename[40];
	struct stat st;
	char buffTime[20];
	struct linkedFileInfoList *next;
};
int initStringDate(char buffTime[], time_t time){
    struct tm *tempTM = malloc(sizeof(struct tm));
    tempTM =localtime(&time);
    strftime (buffTime, 20, "%Y-%m-%d %H:%M:%S", tempTM);
    return EXIT_SUCCESS;
}
static int cmpLinkedFileInfoListByFilename(const void *a, const void *b) 
{ 
    struct linkedFileInfoList *ia = (struct linkedFileInfoList *)a;
    struct linkedFileInfoList *ib = (struct linkedFileInfoList *)b;
    return strcmp(ia->filename,ib->filename);
 
} 

struct linkedFileInfoList *newLinkedFileInfoList(struct dirent *dirent){
	struct linkedFileInfoList *new = malloc(sizeof(struct linkedFileInfoList));
	strncpy(new->filename,dirent->d_name,40);
	char relFilePath[80] ="";
	strcat(relFilePath,curDirName);
	strcat(relFilePath,"/");
	strcat(relFilePath,new->filename);
	stat(relFilePath,&new->st);
	initStringDate(new->buffTime, new->st.st_mtime);
	new->next = NULL;	
	return new;
}

int initLinkedList(char *dirname, bool showInvisible, bool isFileMode, char *argv[], int argc){
	struct linkedFileInfoList *curNode = headP;	
	struct linkedFileInfoList *lastNode = NULL;
	char *tempFileBegin;
	if (isFileMode){
	//Sucht den Dateinamen ohne Ordnernamen und trennt den dirname mit einem NULL-Byte ab
		tempFileBegin = rindex(dirname,'/')+1;
		tempFileBegin[0]='\0';
	}
	DIR *dir = opendir(dirname);
	struct dirent *dirent = readdir(dir);
	while (dirent != NULL){
		if (isFileMode){
			for (int i=1;i<argc;i++){
				if (argv[i][0]!='-'){
					tempFileBegin = rindex(argv[i],'/')+1;
					if (strcmp(tempFileBegin,dirent->d_name)==0){
						curNode = newLinkedFileInfoList(dirent);
						if (headP == NULL)  {headP=curNode; }
						else {lastNode->next = curNode;}
						lastNode = curNode;
						fileCounter++;
						break;
					}
				}
			}
			dirent=readdir(dir);
			continue;
		}
		
		if ((dirent->d_name[0]!='.'||showInvisible)){

			curNode = newLinkedFileInfoList(dirent);
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

int printNode(struct linkedFileInfoList *curNode){
	printf("%-20s %15li Byte \t Modified %s \n",curNode->filename,curNode->st.st_size,curNode->buffTime);
	return EXIT_SUCCESS;
}
int printNodeSimple(struct linkedFileInfoList *curNode){
	printf("%-20s \n",curNode->filename);
	return EXIT_SUCCESS;
}
int printLinkedFileInfoListSimple(){
	if (headP == NULL) {
		return -1;
	}
	struct linkedFileInfoList *curNode = headP;
	while (curNode != NULL){
		printNodeSimple(curNode);
		curNode=curNode->next;	
	}
	return EXIT_SUCCESS;

}

int createSortableArray(){
	curArray = malloc(fileCounter*sizeof(struct linkedFileInfoList));
	struct linkedFileInfoList *curNode = headP;
	for (uint64_t i = 0; i<fileCounter;i++){
		curArray[i]=*curNode;
		curNode=curNode->next;
	}
	return EXIT_SUCCESS;

}
int printSortedArray(){
	struct linkedFileInfoList curNode;
	for (uint64_t i = 0; i<fileCounter;i++){
		curNode = curArray[i];
		printNode(&curNode);
	}
	return EXIT_SUCCESS;
}


int main(int argc, char *argv[]){
	int ret=0;
	int countFileParams=0;	
	bool isFileMode = false;
	bool lParam=false;
	bool aParam=false;
	for (int i = 1;i<argc;i++){
		if (argv[i][0]!='-') {
			strncpy(curDirName,argv[i],40);
			countFileParams++;
		}
		else{			
			for (int j=1;argv[i][j]!='\0';j++){
				if (argv[i][j]=='l'||argv[i][j]=='L') lParam=true;
				else if (argv[i][j]=='a'||argv[i][j]=='A') aParam=true;
				else ret=2;
			}		
		}
	}
	if (countFileParams==1) {
		DIR *dir= opendir(curDirName);	
		if (dir == NULL) {
			if (errno == ENOTDIR) isFileMode = true;
			else return 1;
		} else closedir(dir);
			
	} 
	if (countFileParams>1) {
		isFileMode = true;
	} 

	if (aParam&&!isFileMode){
		initLinkedList(curDirName, true, isFileMode, argv,argc); 
		if (lParam){
			printf("Counter:%i\n", fileCounter);
			createSortableArray();	
			qsort(&curArray[0],fileCounter,sizeof(struct linkedFileInfoList),cmpLinkedFileInfoListByFilename);
			printSortedArray();
		} else 
			printLinkedFileInfoListSimple();
		
	}else if (lParam&&!isFileMode){
		bool showInvi=false;
		if (aParam) showInvi=true;
		initLinkedList(curDirName,showInvi,isFileMode, argv,argc);

 		printf("Verzeichnis ./ sortiert:\n");
		createSortableArray();	
		qsort(&curArray[0],fileCounter,sizeof(struct linkedFileInfoList),cmpLinkedFileInfoListByFilename);
		printSortedArray();
			
	}
	if (!aParam && !lParam && !isFileMode){	
		printf("Open folder:%s\n",curDirName);
		initLinkedList(curDirName,false,isFileMode, argv,argc);
		printLinkedFileInfoListSimple();
	}
	if (isFileMode) {
		initLinkedList(curDirName,aParam,isFileMode, argv,argc);
		if (lParam){
			printf("Counter:%i\n", fileCounter);
			createSortableArray();	
			qsort(&curArray[0],fileCounter,sizeof(struct linkedFileInfoList),cmpLinkedFileInfoListByFilename);
			printSortedArray();
		} else 
			printLinkedFileInfoListSimple();

	}
	printf("return: %i\n",ret);
	return EXIT_SUCCESS;
}// Rückgabewerte ordner nicht gefunden -> 1 ; Parameter existiert nicht -> 2
