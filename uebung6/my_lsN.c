#include <sys/stat.h>
#include <inttypes.h>
#include <stdlib.h>
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

//LinkedList Struct für alle Dateien im Ordner
struct linkedFileInfoList{
	char filename[40];
	struct stat st;
	char buffTime[20];
	struct linkedFileInfoList *next;
};
//Formatierung des Timestamps
int initStringDate(char buffTime[], time_t time){
    struct tm *tempTM = malloc(sizeof(struct tm));
    tempTM =localtime(&time);
    strftime (buffTime, 20, "%Y-%m-%d %H:%M:%S", tempTM);
    return EXIT_SUCCESS;
}
//Vergleichsfunktions zweier Listenknoten für den qsort
static int cmpLinkedFileInfoListByFilename(const void *a, const void *b) 
{ 
    struct linkedFileInfoList *ia = (struct linkedFileInfoList *)a;
    struct linkedFileInfoList *ib = (struct linkedFileInfoList *)b;
    return strcmp(ia->filename,ib->filename);
 
} 
//Initialisierung eines Listenknoten
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
//Initialisierung der verketteten Liste
int initLinkedList(char *dirname, bool showInvisible, bool isFileMode, char *argv[], int argc){
	struct linkedFileInfoList *curNode = headP;	
	struct linkedFileInfoList *lastNode = NULL;
	char *tempFileBegin;
	if (isFileMode){ //Falls Dateien angegben sind
	//Sucht den Dateinamen ohne Ordnernamen und trennt den dirname mit einem NULL-Byte ab
	
		tempFileBegin = rindex(dirname,'/');
		if (dirname[0]=='.'||dirname[0]=='/') tempFileBegin[1]='\0';
		else dirname="./";
	
		
	}

	DIR *dir = opendir(dirname);
	
	struct dirent *dirent = readdir(dir);
	while (dirent != NULL){
		if (isFileMode){//Falls bestimmte Datein angezeigt werden
			for (int i=1;i<argc;i++){ //Durchlaufen der Parameter / Dateien
				if (argv[i][0]!='-'){ 
					//Falls ein anderer Ordner geöffnet wurde, wir der Dateiname extrahiert	
					if (argv[i][0]=='.'||argv[i][0]=='/')	tempFileBegin = rindex(argv[i],'/')+1;	
			
					else tempFileBegin=argv[i];
										
					if (strcmp(tempFileBegin,dirent->d_name)==0){	//Vergleich mit dem filename des aktuellen dirents
						curNode = newLinkedFileInfoList(dirent);	// Falls gleich, wird ein Knoten erstellt
						if (headP == NULL)  {headP=curNode; }
						else {lastNode->next = curNode;}
						lastNode = curNode;
						fileCounter++;
						break;	//bricht for-Schleife ab, da mit nächstem dirent von vorne begonnen werden muss
					}
				}
			}
			dirent=readdir(dir);
			continue;	//springt zur while-Schleife
		}
		//Dies wird nur ausgeführt, !isFileMode gilt
		if ((dirent->d_name[0]!='.'||showInvisible)){	//Falls Datei nicht versteckt ist, oder versteckte Dateien angezeigt werden.

			curNode = newLinkedFileInfoList(dirent);//Knoten für die Datei wird hinzugefügt	
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
//schreibt Knoten mit Dateigröße und Datum der letzten Änderung in stdout
int printNode(struct linkedFileInfoList *curNode){
	printf("%-20s %15li Byte \t Modified %s \n",curNode->filename,curNode->st.st_size,curNode->buffTime);
	return EXIT_SUCCESS;
}
//schreibt Knoten in stdout
int printNodeSimple(struct linkedFileInfoList *curNode){
	printf("%-20s \n",curNode->filename);
	return EXIT_SUCCESS;
}
//gibt gesamte verkette Liste durch und gibt die Knoten einfach aus
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
// erstellt ein sortierbares Array aus den Knoten der verketteten Liste
int createSortableArray(){
	curArray = malloc(fileCounter*sizeof(struct linkedFileInfoList));
	struct linkedFileInfoList *curNode = headP;
	for (uint64_t i = 0; i<fileCounter;i++){
		curArray[i]=*curNode;
		curNode=curNode->next;
	}
	return EXIT_SUCCESS;

}
// gibt sortiertes Array aus
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
	int countFileParams=0;		//Gibt die Anzahl der Parameter ohne '-' zu beginn an
	bool isFileMode = false;	//Gibt an, ob der Modus für die Ausgabe bestimmter Dateien aktiviert ist
	bool lParam=false;		//Gibt an, ob der l-Parameter gesetzt wurde
	bool aParam=false;		//Gibt an, ob der a-Parameter gesetzt wurde
	for (int i = 1;i<argc;i++){
		if (argv[i][0]!='-') {
			strncpy(curDirName,argv[i],40);	//speichert die ersten 40 Zeichen des Ordnernamens oder Filenames ab
			countFileParams++;
		}
		else{			//liest die l & a Parameter aus und setzt die boolschen Werte
			for (int j=1;argv[i][j]!='\0';j++){
				if (argv[i][j]=='l'||argv[i][j]=='L') lParam=true;
				else if (argv[i][j]=='a'||argv[i][j]=='A') aParam=true;
				else ret=1;	//Abbruch: Unbekannter Parameter
			}		
		}
	}
	if (countFileParams==1) {	//Überprüfung der Parameter auf fehlerhafte Eingaben
		DIR *dir= opendir(curDirName);	
		if (dir == NULL) {	//Falls der Parameter kein Ordner ist
			if (errno == ENOTDIR) isFileMode = true;	//Überprüfung ob Parameter eine Datei ist
			else return 2;	//Ansonsten ist Parameter weder Datei, noch Ordner -> 2 wird zurückgegeben
		} else closedir(dir);	//Falls es ein Ordner ist, wird dieser geschlossen
			
	} 
	if (countFileParams>1) {	//Falls es mehr Parameter gibt, ist der Dateienmodus aktiv
		isFileMode = true;
	} 

	if (aParam&&!isFileMode){	//Falls a gesetzt ist und es nicht der Dateimodus ist
		initLinkedList(curDirName, true, isFileMode, argv,argc); 
		if (lParam){	//Falls l gesetzt wurde
			createSortableArray();	
			qsort(&curArray[0],fileCounter,sizeof(struct linkedFileInfoList),cmpLinkedFileInfoListByFilename);
			printSortedArray();
		} else 
			printLinkedFileInfoListSimple();
		
	}else if (lParam&&!isFileMode){	//Falls l gesetzt wurde und es nicht der Dateimodus ist
		bool showInvi=false;
		if (aParam) showInvi=true;
		initLinkedList(curDirName,showInvi,isFileMode, argv,argc);
		createSortableArray();	
		qsort(&curArray[0],fileCounter,sizeof(struct linkedFileInfoList),cmpLinkedFileInfoListByFilename);
		printSortedArray();
			
	}
	if (!aParam && !lParam && !isFileMode){	//Falls keine Parameter gesetzt wurden
		initLinkedList(curDirName,false,isFileMode, argv,argc);
		printLinkedFileInfoListSimple();
	}

	if (isFileMode) {	//Falls der Dateimodus aktiviert wurde
		initLinkedList(curDirName,aParam,isFileMode, argv,argc);
		if (lParam){
			createSortableArray();	
			qsort(&curArray[0],fileCounter,sizeof(struct linkedFileInfoList),cmpLinkedFileInfoListByFilename);
			printSortedArray();
		} else 
			printLinkedFileInfoListSimple();

	}
	return ret;
}// Rückgabewerte ordner nicht gefunden -> 1 ; Parameter existiert nicht -> 2
