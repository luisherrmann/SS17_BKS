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

char *firstEnd = ".";
char *secondEnd = NULL;
struct linkedFileInfoList *headP = NULL;
uint64_t fileCounter = 0;
int64_t *curArray;

struct linkedFileInfoList{
	const char *filename;
	struct stat st;
	int64_t sortValue;
	char buffTime[20];
	struct linkedFileInfoList *next;
};
int initStringDate(char buffTime[], time_t time){
    struct tm *tempTM;
    tempTM =localtime(&time);
    strftime (buffTime, 20, "%Y-%m-%d %H:%M:%S", tempTM);
    return EXIT_SUCCESS;
}
int64_t getSortValue(const char *filename){
	int64_t tempIlast= strToInt(filename,&firstEnd,(uint64_t)36);
	printf("%s\n",filename);	
	printf("Erster Wert ermittelt: %li\n",tempIlast);
	printf("Position des . :%i\n",strcspn(filename,"."));
	if (tempIlast==0) secondEnd=".";
	int64_t tempDotHidden = strToInt(filename+strcspn(filename,".")+1
	,&secondEnd,(uint64_t)36);

	printf("Zweiter Wert ermittelt: %li\n",tempDotHidden);
	firstEnd=".";
	secondEnd=NULL;		
	int64_t curI=tempIlast;
	while (curI>=tempIlast&&curI>0){
		tempIlast=curI;
		curI*=36;
	}
printf("return Wert: %li \n",tempIlast+tempDotHidden);
	return tempIlast+tempDotHidden;
}
struct linkedFileInfoList *newLinkedFileInfoList(struct dirent *dirent){
	struct linkedFileInfoList *new = malloc(sizeof(struct linkedFileInfoList));
	struct stat stS;
	new->filename = dirent->d_name;
	stat(dirent->d_name,&stS);
	new->st = stS;
	char buffTime[20];
	initStringDate(buffTime, stS.st_mtime);
	strncpy(new->buffTime,buffTime,20);
	new->sortValue = getSortValue(new->filename);
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
		
		if (strcmp(dirent->d_name,".")!=0 && strcmp(dirent->d_name,"..")!=0){// || showInvisible){

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
		printLinkedFileInfoList();


	}
	if (argc == 2){
		if (argv[1][0] == '-'){
		switch(argv[1][1]){
			case 'a': initLinkedList("./", true); printLinkedFileInfoList(); break;
			case 'l': {
				bool showInvi=false;
				if (argv[1][2]=='a'||argv[1][2]=='A') {printf("A!\n");showInvi=true;}
				initLinkedList("./",showInvi);
 				printf("Verzeichnis ./ sortiert:\n");
				createSortableArray();	
				sort(fileCounter,curArray);
				printSortedLS(); 
				break;}
			
		}}else {
			printf("Open folder:%s\n",argv[1]);
			char tempPath[] = "";
			strcat(tempPath,argv[1]);
			initLinkedList(tempPath,false);
			printLinkedFileInfoList();
		}
	}		
	return EXIT_SUCCESS;
}
