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

extern int64_t strToInt(const char*, char**, uint8_t);
extern void sort(uint64_t len, int64_t a[len]);

char* end = NULL;
struct linkedFileInfoList *headP = NULL;
uint64_t fileCounter = 0;

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
	struct stat stS;
	stat(dirent->d_name,&stS);
	
	new->st = stS;
	new->filename = dirent->d_name;

	new->dirPos = telldir(dir);
	new->sortValue = getSortValue(new->filename);
	
	
	printf("File: %s \t SortValue:%"PRIu64"\n",new->filename,new->sortValue);
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
int printLinkedFileInfoList(){
	if (headP == NULL) {
		printf("HeadP == NULL\n");	
		return -1;
	}
	struct linkedFileInfoList *curNode = headP;
	while (curNode != NULL){
		printf("%s\t\t\t\t%li Byte\n",curNode->filename,curNode->st.st_size);

		
		curNode=curNode->next;	
	}
	return EXIT_SUCCESS;

}
int64_t *curArray;
int createSortableArray(){
	curArray = (int64_t *)malloc(fileCounter*sizeof(int64_t));
	struct linkedFileInfoList *curNode = headP;
	for (uint64_t i = 0; i<fileCounter;i++){
		curArray[i]=curNode->sortValue;
		printf("curArray[i] = %"PRIu64"\n",curArray[i]);
		curNode=curNode->next;
	}
	return EXIT_SUCCESS;

}
static inline void printArray(int64_t* to_show, uint64_t len) {
	for(uint64_t i=0; i<len; i++) {
		if(i == 0) { // erstes Element
			printf("Array: %"PRId64", ", to_show[i]);
		} else if(i == (len-1)) { // letztes Element
			printf("%"PRId64"\n", to_show[i]);
		} else {
			printf("%"PRId64", ", to_show[i]);
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
		createSortableArray();
		printArray(curArray,fileCounter);
		sort(fileCounter,curArray);
		printArray(curArray,fileCounter);

	}
	if (argc == 2){
		if (argv[1][0] == '-'){
		switch(argv[1][1]){
			case 'a': initLinkedList("./", true); printLinkedFileInfoList(); break;
			case 'l': break;
		}}else {
			char tempPath[] = "./";
			strcat(tempPath,argv[1]);
			initLinkedList("./",false);
			printLinkedFileInfoList();
		}
	}		
	return EXIT_SUCCESS;
}
