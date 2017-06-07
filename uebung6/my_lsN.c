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


struct linkedFileInfoList *headP = NULL;
uint64_t fileCounter = 0;
struct linkedFileInfoList *curArray;
char curDirName[40] = "./";

struct linkedFileInfoList{
	char filename[40];
	struct stat st;
	char buffTime[20];
	struct linkedFileInfoList *next;
};
int initStringDate(char buffTime[], time_t time){
    struct tm *tempTM;
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
	struct stat stS;
	strncpy(new->filename,dirent->d_name,40);
	stat(dirent->d_name,&stS);
	new->st = stS;
	char buffTime[20];
	initStringDate(buffTime, stS.st_mtime);
	strncpy(new->buffTime,buffTime,20);
	new->next = NULL;	
	return new;
}

int initLinkedList(char *dirname, bool showInvisible){
	DIR *dir = opendir(dirname);
	struct dirent *dirent = readdir(dir);
	struct linkedFileInfoList *curNode = headP;
	struct linkedFileInfoList *lastNode = NULL;
	int pointD = 0;
	while (dirent != NULL){
		
		if (dirent->d_name[0]!='.'||showInvisible){

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
/*	if (argc > 3){
		const char message[] ="Zu viele Parameter für my_ls.\n";
		write(STDERR_FILENO,message,sizeof(message));
		return EXIT_FAILURE;
	}else */
	if (argc==1){
		initLinkedList(curDirName,false);
		printLinkedFileInfoListSimple();


	}
	bool lParam=false;
	bool aParam=false;
	for (int i = 1;i<argc;i++){
		if (argv[i][0]!='-') strncpy(curDirName,argv[i],40);
		else{
			for (int j=1;j<=2;j++){
				if (argv[i][j]=='l'||argv[i][j]=='L') lParam=true;
				if (argv[i][j]=='a'||argv[i][j]=='A') aParam=true;
			}		
		}
	}
	if (aParam){
		initLinkedList(curDirName, true); 
		if (lParam){
			createSortableArray();	
			qsort(&curArray[0],fileCounter,sizeof(struct linkedFileInfoList),cmpLinkedFileInfoListByFilename);
			printSortedArray();
		} else 
			printLinkedFileInfoListSimple(); 
		
	}
	if (lParam){
		bool showInvi=false;
		if (aParam) showInvi=true;
		initLinkedList(curDirName,showInvi);
 		printf("Verzeichnis ./ sortiert:\n");
		createSortableArray();	
		qsort(&curArray[0],fileCounter,sizeof(struct linkedFileInfoList),cmpLinkedFileInfoListByFilename);
		printSortedArray();
			
	}
	if (!aParam && !lParam){	
		printf("Open folder:%s\n",curDirName);
		initLinkedList(curDirName,false);
		printLinkedFileInfoListSimple();
	}
			
	return EXIT_SUCCESS;
}// Rückgabewerte ordner nicht gefunden -> 1 ; Parameter existiert nicht -> 2
