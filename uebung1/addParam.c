#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#define RED "\033[31m"
#define WHITE "\033[39m"

/** Die Funktion checkConvertRes(char* string, char* err) bekommt den Pointer auf einen String (string) und den Pointer auf das zuletzt gelesene Zeichen (err) übergeben. Wenn diese Funktion nach einer strtol Funktion mit den entsprechenden Parametern aufgerufen wird, überprüft sie ob das zuletzt korrekt gelesene Zeichen des Strings '0' ist, da Strings mit '0' terminieren. Wenn dies der Fall ist und der Parameter kein '0' war, dann wurden nicht nur Ziffern übergeben. Es wird eine Fehlermeldung ausgegeben und das Programm beendet.

*/
void checkConvertRes(char* string,char* err){
	if (!(string!='\0' && *err=='\0')){
		printf(RED "Eingabefehler: Als Parameter sind nur ganze Zahlen im Dezimalsystem zulässig.\n" WHITE);		
		exit(EXIT_FAILURE);
	}
}
/** Die Funktion checkParamCount(int argc) überprüft, ob mindestens zwei Parameter (zusätzlich zum Dateinamen) übergeben werden. Wenn dies nicht der Fall ist, dann wird ein entsprechender Fehler ausgegeben und das Programm wird beendet.
*/
void checkParamCount(int argc){
	if (argc<3){
		printf(RED "Eingabefehler: Es müssen mindestens zwei Parameter übergeben werden.\n" WHITE);
		exit(EXIT_FAILURE);
	}
}
/** Die main-Funktion wird automatisch aufgerufen und addiert die übergebenen Parameter. Dazu wird das String Array der Parameter durchlaufen und die einzelnen Parameter mit der strtol-Funktion in long convertiert und anschließend aufaddiert. Die Funktion schreibt '1' wenn die Summe der Parameter dem letzten Parameter entspricht und ansonsten '0'. Abbruchbedingungen sind in die Funktionen checkParamCount(int argc) und checkConvertRes(char* string, char* err) ausgelagert. 
*/
int main(int argc, char *argv[]){
	checkParamCount(argc);
	uint64_t temp = 0;
	uint64_t tempRes = 0;
	for (int i=1;i<=argc-1;i++){
	//Überspringe den ersten Parameter String *argv[0], da dieser in der Regel den Pfad des Programms enthält
		char* err;
		tempRes = strtol(argv[i],&err,10);
		checkConvertRes(argv[i],err);
		if (i!=argc-1)
			temp =temp + tempRes;
	}
	if (temp == tempRes) printf("%c\n",'1');		
	else printf("%c\n",'0');
	return EXIT_SUCCESS;
}

